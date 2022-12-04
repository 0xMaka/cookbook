# a simple, working example of a learning agent 
# using baseline3, keras, tensorflow and open ai gym

from baselines.common.atari_wrappers import make_atari, wrap_deepmind
import numpy as np
import tensorflow as tf
from tensorflow import keras
from tensorflow.keras import layers
# for training gym
import gym
from ale_py import ALEInterface
from ale_py.roms import Breakout

ale = ALEInterface()
ale.loadROM(Breakout)

# config params for set up
seed = 42
gamma = 0.99     # discount factor for past rewards
epsilon = 1.0    # greed params
emin  = 0.1      # min greed param
emax = 1         # max greed param

# threshold interval
eint = emax - emin  # amount to reduce random action rate by
nbatch = 32  # batch size from replay buffer
max_steps = 16 * 8 # max steps per episode

# using baseline atari for deepmind helper functions
env = gym.make('ALE/Breakout-v5', render_mode = 'human')

# wrap frames
env = wrap_deepmind(env, frame_stack = 1, scale = 1)
env.seed(seed)

# the deep q network
# ------------------
nactions = 4
def fetch_model() -> object:
  # network defined by deep mind paper
  # convolutions on the frames of the screen
  inputs = layers.Input(shape=(84,84,4))
  layer0 = layers.Conv2D(32, 8, strides=4, activation='relu')(inputs)
  layer1 = layers.Conv2D(64, 4, strides=2, activation='relu')(layer0)
  layer2 = layers.Conv2D(64, 3, strides=1, activation='relu')(layer1)
  layer3 = layers.Flatten()(layer2)
  layer4 = layers.Dense(512, activation = 'relu')(layer3)
  action = layers.Dense(nactions, activation = 'relu')(layer4)
 
  return keras.Model(inputs=inputs, outputs=action)

# first model makes the predictions for q values used to take action
model = fetch_model() 
model.load_weights('brock_weights.h5')

# build a target model for the prediction for rewards
# the weights of the target mem get updated every 80 * 16 steps
# loss between q val is calulated, target q val is stable

mmodel = fetch_model()

# trainer
# -------
# note that rnsprop was used in deep mind paper
op = keras.optimizers.Adam(learning_rate=2e5, clipnorm=1.0)

# buffers for exp
action_history = []
state_history = []
state_next_history = []

rewards_history = []
done_history = []
episode_reward_history = []

running_reward = 0
episode_count = 0
frame_count = 0

# n frames to take actions and observe input
epsilon_random_frames = 50000
# n frames for exploration
epsilon_greedy_frames = 1000000.0
# max replay length
# note: deep min paper suggests 1000000
max_mem_len = 100000

# train model after every so many actions
update_after_actions = 4
# update target network after every so many
update_target_network = 10000
#using huber loss for stability
loss_function = keras.losses.Huber()

# helper
#term, trunc = 0, 0
#def done(a,b) -> bool:
#  return a or b

# main
while 1:
  state = np.array(env.reset())
  episode_reward = 0

  for step in range(1, max_steps):
    env.render # visualise attempts #
    frame_count += 1

    # greed for exploration
    if (frame_count < epsilon_random_frames or epsilon > np.random.rand(1)[0]):
      action = np.random.choice(nactions) # take random action
    else:
      # predict q val from action state
      state_tensor = tf.convert_to_tensor(state)
      state_tensor = tf.expand_dims(state_tensor, 0)
      action_probs = model(state_tensor, training=False)
      action = tf.argmax(action_probs[0]).numpy() # perform optimal known
  
    # decay chance of taking random action
    epsilon -= eint / epsilon_greedy_frames
    epsilon = max(epsilon, emin)

    # apply the sampled actions in our environment
    state_next, reward, done, info = env.step(action)
    state_next = np.array(state_next)

    episode_reward += reward

    # save actions and state in replay buffer
    action_history.append(action)
    state_history.append(state)
    state_next_history.append(state_next)
    done_history.append(done) #(trunc,term))
    rewards_history.append(reward)
    state += state_next

    # update every n frames once batch size is > 32
    if frame_count % update_after_actions == 0 and len(done_history) > nbatch:
      # get indices of smaples for replay buffer
      indices = np.random.choice(range(len(done_history)), size = nbatch)

      # sample the replay buffer 
      state_sample = np.array([state_history[i] for i in indices])
      state_next_sample = np.array([state_next_history[i] for i in indices])
      rewards_sample = np.array([rewards_history[i] for i in indices])
      action_sample = np.array([action_history[i] for i in indices])
      done_sample = tf.convert_to_tensor([float(done_history[i]) for i in indices])

      # build the updated q values
      # using target model for stability
      future_rewards = mmodel.predict(state_next_sample)
      # qval = (reward + discount factor) * expected future rewards
      updated_q_values = rewards_sample + gamma * tf.reduce_max(future_rewards, axis=1)

      # if final frame set the last value -1
      updated_q_values = updated_q_values * (1 - done_sample) - done_sample 

      # mask so we only calculate loss on updated q values
      masks=tf.one_hot(action_sample, nactions)


      with tf.GradientTape() as tape:
        # train model on states and updated qvalues
        qval = model(state_sample)

        # apply masks to vals to get val for action taken
        qact = tf.reduce_sum(tf.multiply(qval, masks), axis=1)

        # calculate loss between q val and old val
        loss = loss_function(updated_q_values, qact)

      # backpropergation
      grads = tape.gradient(loss, model.trainable_variables)
      op.apply_gradients(zip(grads, model.trainable_variables))

      if (frame_count % update_target_network == 0) :
        # update target net with new weights
        mmodel.set_weights(model.get_weights())

        # log details
        template = '[+] running reward: {:.2f} at episode {}, frame count {}'.format(
          running_reward, episode_reward, frame_count
        )
        print (template)

      # prunt state and reward history
      if (len(rewards_history) > max_mem_len):
        del rewards_history[:1]
        del state_history[:1]
        del state_next_history[:1]
        del action_history[:1]
        del done_history[:1]

      if done: break

    # update running reward to check condition for solving
    episode_reward_history.append(episode_reward)
    if (len(episode_reward_history) > 100):
      del episode_reward_history[:1]

    running_reward = np.mean(episode_reward_history)
    episode_count += 1
    
    model.save_weights('brock_weights.h5')

    if (running_reward > 10):
      print(f'[>] solved at episode: {episode_count}')
      break

