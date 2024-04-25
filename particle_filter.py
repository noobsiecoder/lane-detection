import numpy as np
import math

class Particle_filter():
    def __init__(self, variance_bottom, variance_top) -> None:
        
        self.variance_bottom = variance_bottom # std_dev_bottom ** 2 # 900 #10 #5
        self.variance_top = variance_top # std_dev_top ** 2 # 800 #100 #10
        pass

    def get_gaussian(self, value, mean, variance):

        """Function that returns the normal pdf"""

        denom = math.sqrt(2*math.pi*variance)
        val = ((value - mean) ** 2)/(-2 * variance)
        return math.exp(val)/denom

    def get_estimate(self, particles, reading):
        
        # format of reading and particles: [x0, y0, x1, y1]
        
        # Checking for empty inputs
        if len(particles) < 1 or len(reading) < 1:
            return []
        
        probs = []

        # Getting the errors
        for particle in particles:
            err0 = self.get_gaussian(particle[0], reading[0], variance=self.variance_bottom)
            err1 = self.get_gaussian(particle[2], reading[2], variance=self.variance_top)
            
            probs.append(err0 * err1)

            y0 = particle[1]
            y1 = particle[3]
            pass
        
        # Getting the Probability Mass Function and Normalizing the values        
        probs = np.array(probs)
        prob_sum = np.sum(probs)
        pmf = probs/prob_sum

        indices = np.arange(len(particles))
        
        try:
            # Updating the particles using the PMF
            updated_particle_indices = np.random.choice(indices, size=len(particles), p=pmf)
        except:
            # print("probs: ", probs)
            probs = []
            x0_list2 = []
            x1_list2 = []
            
            print("reading: ", reading)
            print("\n----------\n")
            for particle in particles:
                print("particle: ", particle)
                err0 = self.get_gaussian(particle[0], reading[0], variance=self.variance_bottom)
                err1 = self.get_gaussian(particle[2], reading[2], variance=self.variance_top)
                
                print("err0: ", err0)
                print("err1: ", err1)
                probs.append(err0 * err1)
                x0_list2.append(particle[0])
                x1_list2.append(particle[2])
                y0 = particle[1]
                y1 = particle[3]
                pass
            # exit()

            x0 = int(np.median(np.array(x0_list2)))
            x1 = int(np.median(np.array(x1_list2)))

            return [x0, y0, x1, y1]
            # return []

        x0_list = []
        x1_list = []

        for i in updated_particle_indices:
            x0_list.append(particles[i][0])
            x1_list.append(particles[i][2])
        
        x0 = sum(x0_list) // len(x0_list)
        x1 = sum(x1_list) // len(x1_list)
        
        # x0 = int(np.median(np.array(x0_list)))
        # x1 = int(np.median(np.array(x1_list)))

        return [x0, y0, x1, y1]