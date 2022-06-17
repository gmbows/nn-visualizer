# nn-visualizer
![img](https://raw.githubusercontent.com/gmbows/nn-visualizer/master/window.PNG)

## Parameters 
The program will import network parameters from the file `params.txt`.  The first line of the file must be the topology of network, and the second must be the eta, then alpha values seperated by a space. <br>
Parameters can be changed during training.  `E` and `W`, `S` and `A` keys will increase and decrease eta and alpha values respectively.

## Training
The network imports training data from `training_data.txt`.  A python file is included that will generate various different data sets, but as long as it follows the correct format you can use your own data.

## Display
Use the `g` key to toggle between displaying neuron activation values and gradients.  <br>
Use the up and down arrow keys to increase or decrease timescale. <br> 
Bias neurons are indicated in blue, connection weights and neuron activation values are displayed on a blue-red-green spectrum (close to -1 is blue, close to 0 is red, close to 1 is green).   <br>
Displays number of trials, average mean squared error over last 1000 trials, eta, and alpha values. <br>
