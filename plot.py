import matplotlib.pyplot as plt
import numpy as np
from scipy.interpolate import make_interp_spline, BSpline

def create_bar_chart(title,xlabel,ylabel,xdata,ydata):
	plt.figure(figsize=(10,5))

	plt.xlabel(xlabel)
	plt.ylabel(ylabel)
	plt.title(title)

	plt.plot(xdata, ydata)
	plt.savefig("chart.png",dpi=100)
	
def generate_training_chart(filename):
	f = open("training.txt","r")
	data = f.read().split(" ")[0:-1]
	f.close()
	
	xdata = [i*100 for i in range(len(data))]
	ydata = sorted(data)
	xdata.reverse()

	ctitle = "Training data"
	xlabel = "Trial"
	ylabel = "Error"
	create_bar_chart(ctitle,xlabel,ylabel,xdata,ydata)
	
generate_training_chart("training.txt")