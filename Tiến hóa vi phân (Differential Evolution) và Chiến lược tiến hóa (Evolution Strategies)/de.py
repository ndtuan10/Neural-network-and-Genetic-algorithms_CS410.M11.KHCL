import numpy as np

def DE(fobj, dimension, bounds, F_scale, cross_prob, popsize, max_evals):

    lower_bound, upper_bound = np.asarray(bounds).T

    eps = 0.00001

    diff = np.fabs(lower_bound - upper_bound)

    pop = lower_bound + diff * np.random.rand(popsize, dimension)

    fitness = np.asarray([fobj(ind) for ind in pop])
    num_eval = 1
    
    best_idx = np.argmin(fitness)
    best = pop[best_idx]

    results = []
    all_pops = []
    results.append((np.copy(best), fitness[best_idx], num_eval))
    all_pops.append(np.copy(pop))
    generation_count = 0
    
    while True:
        if num_eval > max_evals:
            break
        for i in range(popsize):

            idxes = [idx for idx in range(popsize) if idx != i]
            a, b, c = pop[np.random.choice(idxes, 3, replace=False)]
            mutant = np.clip(F_scale*(b - c) + a, lower_bound, upper_bound)

            cross_points = np.random.rand(dimension) < cross_prob
            if not np.any(cross_points):
                cross_points[np.random.randint(0, dimension)] = True
            
            trial = np.where(cross_points, mutant, pop[i])

            f = fobj(trial)
            num_eval += 1

            if f < fitness[i]:
                pop[i] = trial
                fitness[i] = f 
                if f < fitness[best_idx]:
                    best = trial
                    best_idx = i

        results.append((np.copy(best), fitness[best_idx], num_eval))
        all_pops.append(np.copy(pop))

        if fobj(best) < eps:
            num_eval += 1
            break

        generation_count += 1

    return results, all_pops, generation_count

F_scale=0.8 # hệ số scale F (0,1)
cross_prob=0.7 # xác suất hai cá thể đem đi lai ghép
#dimension = 2
#max_evals = 1e5
#fobj
#seed_number = 18520186
#popsize = 32 # kích thước quần thể
#lower_bound = -6
#upper_bound = 6