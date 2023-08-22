# 如果所有的reward都是正的，那最后gain也是正的，不太好，需要normliazation
# 一种方法就是每个reward减去一个baseline
# 还有一种就是减去V(s(t)),也就是下面的critic生成的
# 为什么合理呢？因为，同一个state，会有不同概率的action，因此gain也是概率分布的
# 如果大于0，则代表好于平均情况，说明这个action好
# 这种idea有点不太好，因为有很多路径获取当前的gain，平均减去一个情况不是很合理
# 所以我们应该平均减去平均
# 最终gain = r(t) + V(s(t+1)) - V(s(t)) under the theta

# on-policy 就是与环境交互的和去训练的acotr是同一个， 
#off就是这两个不一样，这样下一个theta就可以使用以前的训练结果，就不会很耗时，经典的就是PPO

# critic also called value function
# actually, it is the gain(discounted cumulation) 
# given the parameters and current observation
# but the difference is that it can predict rather than the game terminates

# how to train a critic
# 1. let actor interact with env until it is over, then
# get the training data then end-to-end train
# 2. temporal-difference approach
# since V(s(t)) = gamma*V(s(t+1)) + r(t)
# we can train the difference is close to r(t)  


'''
reward shaping
inverse rl
'''
def train():
    for i in range(100):
        done = False
        # get s and a sequence
        # compute gain
        # update the parameters according to the loss function
class config:
    def __init__(self) -> None:
        self.room_num = 20
        self.max_temperature = 25
        self.min_temperature = 20
        self.gamma = 0.9 # discount factor
        self.lr = 0.1 # learning rate
        pass

class env:
    def __init__(self,cfg) -> None:
        pass
