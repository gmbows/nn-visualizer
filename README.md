# nn-visualizer
![img](https://raw.githubusercontent.com/gmbows/nn-visualizer/master/window.PNG)
A small neural network visualizer built in C++ using SDL2. <br>
Use `g` key to toggle between displaying neuron activation values and gradients.  <br>
Use the up and down arrow keys to increase/decrease timescale. <br> 
Bias neurons are indicated in blue, connection weights and neuron activation values are displayed on a red-green spectrum (closer to 0 is red, closer to 1 is green).   <br>
Displays number of trials, average mean squared error over last 100 trials, eta, and alpha values. <br>
Trains to resemble an arbitrary logic gate.
