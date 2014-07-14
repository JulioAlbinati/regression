#ifndef OPTIMIZED_SELECTION_SGP

#define OPTIMIZED_SELECTION_SGP

#include "OptimizedCrossSGP.hpp"

class OptimizedSelectionSGP : public OptimizedCrossSGP
{
public:
	inline OptimizedSelectionSGP(unsigned int pop_size, unsigned int num_iter, unsigned int max_height, unsigned int mut_height, unsigned int tour_size, 
		  double mut_rate, double mut_step, short mode_init, unsigned int num_terminals, const std::vector<std::string>& operators) : 
		  OptimizedCrossSGP(pop_size, num_iter, max_height, mut_height, tour_size, mut_rate, mut_step, mode_init, num_terminals, operators), 
		  budget(pop_size * num_iter) {}

	inline virtual void run(const Dataset& train, const Dataset& test)
	{
		srand(time(NULL));
		initialize_pop(train, test);

		std::pair<unsigned int, double> best_at_train;
                while (budget > 0)
                {
                        std::cout << "Evaluations remaining: " << budget <<  std::endl;
                        iteration(train, test);

                        best_at_train = cur_pop->best(train);
                        std::vector<Statistics> fitness_train = cur_pop->evaluate(train);
                        std::cout << "RMSE (train): " << fitness_train[best_at_train.first].rmse << std::endl;
                        std::cout << "MSE (train): " << fitness_train[best_at_train.first].mse << std::endl;
                        std::cout << "MAE (train): " << fitness_train[best_at_train.first].mae << std::endl;
                        std::cout << "Total error (train): " << fitness_train[best_at_train.first].total_error << std::endl;

                        std::vector<Statistics> fitness_test = cur_pop->evaluate(test, false);
                        std::cout << "RMSE (test): " << fitness_test[best_at_train.first].rmse << std::endl;
                        std::cout << "MSE (test): " << fitness_test[best_at_train.first].mse << std::endl;
                        std::cout << "MAE (test): " << fitness_test[best_at_train.first].mae << std::endl;
                        std::cout << "Total error (test): " << fitness_test[best_at_train.first].total_error << std::endl;

                        std::cout << "Size: " << (*cur_pop)[best_at_train.first].size() << std::endl;
                }
	}

protected:
	unsigned int budget;

	virtual void iteration(const Dataset& train, const Dataset& test);
	virtual const Individual& best_match(const Individual& ind, const Dataset& train);
};

#endif
