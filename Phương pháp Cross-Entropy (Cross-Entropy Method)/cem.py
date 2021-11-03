import numpy as np

def CEM(fobj, dimensions, bounds, popsize, num_elite, sigma_init, seed_number):
    np.random.seed(seed_number)
    eps = 1e-4
    lower_bound, upper_bound = np.asarray(bounds).T
    sigma = sigma_init * np.eye(dimensions)

    diff = np.fabs(lower_bound - upper_bound)
    n_evals = 0
    num_evals = [0]
    # mu = np.random.rand(dimensions) - (upper_bound + 1)
    mu = lower_bound + diff * np.random.rand(dimensions)
    generation_count = 0
    all_mu = []
    all_sigma = []
    all_offspring = []
    all_elite = []
    all_fitness = []
    
    
    while True:
    # for i in range(10000):
        if n_evals > max_evals:
            break
        all_mu.append(mu)
        all_sigma.append(sigma)

        x = np.random.multivariate_normal(mu, sigma, popsize)
        # print(np.sum(x))
        all_offspring.append(x)
        fitness = np.array([fobj(x[i]) for i in range(popsize)])
        n_evals += popsize
        best_fitness = max(fitness) 
        all_fitness.append(best_fitness)
        # print(x)
        if best_fitness < eps or np.sum(x) > 1e150 or np.sum(x) < -1e150:
            break

        elite_idx = fitness.argsort()[:num_elite]
        all_elite.append(elite_idx)
        mu = np.mean(x[elite_idx], axis=0)

        sigma = np.zeros_like(sigma)
        for i in range(num_elite):
            z = x[elite_idx[i]] - mu
            z = z.reshape(-1, 1)
            # print(num_evals)
            # sigma += tf.matmul(z.T, z)
            # sigma += (z.T * z)
            sigma += (z.T @ z)

        sigma *= (1/num_elite)
        generation_count += 1
        num_evals.append(n_evals)

    all_mu.append(mu)
    best_results = mu.copy()
    best_fitness = fobj(mu)
    return all_mu, all_fitness, num_evals, generation_count

#dimension = 2
max_evals = 1e5
#fobj = Sphere_f
#seed_number = 18520186
#popsize = 32
#lower_bound = -6
#upper_bound = 6
#num_elite = 10
#sigma_init = 4