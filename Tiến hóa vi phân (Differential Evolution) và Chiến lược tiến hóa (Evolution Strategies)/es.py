import numpy as np

# (1+lamda)ES
def ES(fobj, bounds, sigma_init, c_inc, c_dec, popsize, max_evals, dimension):

    lower_bound, upper_bound = np.asarray(bounds).T

    eps = 0.00001

    diff = np.fabs(lower_bound - upper_bound)

    mu = lower_bound + diff * np.random.rand(dimension)
    mu_fitness = fobj(mu)
    num_eval = 0

    results = []
    all_pops = []
    results.append((np.copy(mu), mu_fitness, num_eval))
    generation_count = 0
    sigma = sigma_init
    
    while True:
        max_evals = 10000 if popsize >= 512 else 5000
        if num_eval > max_evals:
            break
        epsilon = np.random.randn(popsize, dimension)
        offspring = mu + sigma * epsilon
        offspring = np.clip(offspring, lower_bound, upper_bound)
        offspring_fitness = np.asarray([fobj(offspring[i]) for i in range(popsize)])
        num_eval += popsize
        
        best_idx = offspring_fitness.argmin()
        best_fitness = offspring_fitness[best_idx]
        best_offspring = offspring[best_idx]

        if best_fitness <= mu_fitness:
            mu = best_offspring.copy()
            mu_fitness = best_fitness
            sigma *= c_inc 
        else:
            sigma *= c_dec
        
        results.append((np.copy(mu), mu_fitness, num_eval))
        all_pops.append(np.copy(offspring))
        if abs(mu_fitness) < eps:
            break
        generation_count += 1

    return results, all_pops, generation_count

sigma_init = 1.0 # kích thước step-size 
c_inc = 1.1 # hệ số tăng
c_dec = 0.6 # hệ số giảm
#dimension = 2
#fobj = Sphere_f
#max_evals = 1e5
#seed_number = 18520186
#lower_bound = -6
#upper_bound = 6
#popsize = 32
#bounds = [(lower_bound, upper_bound)]*dimension