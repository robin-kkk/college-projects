import tensorflow as tf
import numpy as np
import matplotlib.pyplot as plt
from tensorflow.examples.tutorials.mnist import input_data

np.random.seed(20191201)
tf.set_random_seed(20191201)

# Input Layer
mnist = input_data.read_data_sets("./data/", one_hot=True)
x = tf.placeholder(tf.float32, [None, 784])
x_image = tf.reshape(x, [-1,28,28,1])

# Hidden Layer 1
W_conv1 = tf.Variable(tf.truncated_normal([3,3,1,32], stddev=0.1))
h_conv1 = tf.nn.conv2d(x_image, W_conv1, strides=[1,1,1,1], padding='SAME')
b_conv1 = tf.Variable(tf.constant(0.1, shape=[32]))
h_conv1_cutoff = tf.nn.relu(h_conv1 + b_conv1)
h_pool1 = tf.nn.max_pool(h_conv1_cutoff, ksize=[1,2,2,1],
				strides=[1,2,2,1], padding='SAME')

# Hidden Layer 2
W_conv2 = tf.Variable(tf.truncated_normal([3,3,32,64], stddev=0.1))
h_conv2 = tf.nn.conv2d(h_pool1, W_conv2, strides=[1,1,1,1], padding='SAME')
b_conv2 = tf.Variable(tf.constant(0.1, shape=[64]))
h_conv2_cutoff = tf.nn.relu(h_conv2 + b_conv2)
h_pool2 = tf.nn.max_pool(h_conv2_cutoff, ksize=[1,2,2,1],
				strides=[1,2,2,1], padding='SAME')

# Fully Connected Layer
num_units1 = 7*7*64
num_units2 = 1024

h_pool_flat = tf.reshape(h_pool2, [-1, num_units1])
w2 = tf.Variable(tf.truncated_normal([num_units1, num_units2]))
b2 = tf.Variable(tf.constant(0.1, shape=[num_units2]))
FC = tf.nn.relu(tf.matmul(h_pool2_flat, w2) + b2)

keep_prob = tf.placeholder(tf.float32)
FC_drop = tf.nn.dropout(FC, keep_prob)

# Output Layer
w0 = tf.Variable(tf.zeros([num_units2, 10]))
b0 = tf.Variable(tf.zeros([10]))
p = tf.nn.softmax(tf.matmul(FC_drop, w0) + b0)

# Optimizer & Evaluator
t = tf.placeholder(tf.float32, [None, 10])
loss = -tf.reduce_sum(t * tf.log(p))
train_step = tf.train.AdamOptimizer(0.0001).minimize(loss)
correct_prediction = tf.equal(tf.argmax(p, 1), tf.argmax(t, 1))
accuracy = tf.reduce_mean(tf.cast(correct_prediction, tf.float32))

# Training
sess = tf.InteractiveSession()
sess.run(tf.global_variables_initializer())

for i in range(1, 2001):
    batch_xs, batch_ts = mnist.train.next_batch(50)
    sess.run(train_step,feed_dict={x:batch_xs, t:batch_ts, keep_prob:0.5})
    if i % 100 == 0:
        loss_vals, acc_vals = [], []
        for c in range(4):
            start = int(len(mnist.test.labels) / 4 * c)
            end = int(len(mnist.test.labels) / 4 * (c+1))
            loss_val, acc_val = sess.run([loss, accuracy],
                                    feed_dict={x:mnist.test.images[start:end],
                                                t:mnist.test.labels[start:end],
                                                keep_prob:1.0})
            loss_vals.append(loss_val)
            acc_vals.append(acc_val)

        loss_val = np.sum(loss_vals)
        acc_val = np.mean(acc_vals)
        print ('Step: %d, Loss: %f, Accuracy: %f' % (i, loss_val, acc_val))


'''
Step: 100, Loss: 5082.247070, Accuracy: 0.872700
Step: 200, Loss: 3290.780273, Accuracy: 0.906800
Step: 300, Loss: 2661.613770, Accuracy: 0.925000
Step: 400, Loss: 2259.271973, Accuracy: 0.931700
Step: 500, Loss: 2052.433105, Accuracy: 0.939300
Step: 600, Loss: 1868.204346, Accuracy: 0.945200
Step: 700, Loss: 1724.504395, Accuracy: 0.950800
Step: 800, Loss: 1561.319702, Accuracy: 0.954000
Step: 900, Loss: 1497.562622, Accuracy: 0.954600
Step: 1000, Loss: 1357.353027, Accuracy: 0.960600
Step: 1100, Loss: 1269.200928, Accuracy: 0.962200
Step: 1200, Loss: 1214.817017, Accuracy: 0.963500
Step: 1300, Loss: 1212.025269, Accuracy: 0.963600
Step: 1400, Loss: 1146.572754, Accuracy: 0.966200
Step: 1500, Loss: 1085.940796, Accuracy: 0.967300
Step: 1600, Loss: 1011.688293, Accuracy: 0.971000
Step: 1700, Loss: 1018.463623, Accuracy: 0.970500
Step: 1800, Loss: 980.152405, Accuracy: 0.971200
Step: 1900, Loss: 923.292725, Accuracy: 0.973000
Step: 2000, Loss: 920.533447, Accuracy: 0.971300
'''