# comptonUtility
 A series of codes which can be helpful for our Compton lab experience. 

 In particular, this code allows you to:
 - fit data with gaussian peaks and exponential/erfc-like/linear backgrounds;
 - check for linearity of certain sets of data;
 - perform an optimization of the instrumentation used throughout the experience (for example, putting the FWHM of the observed gaussian peaks as a function of certain parameters);
 - etc.

To compile, on your terminal:
    **g++ compton_program.C `root-config --glibs --cflags` -o compton**
and to execute, you must type in the following (or else the program won't start):
    **./compton name_of_file.txt "Label for the x-axis" "Label for the y-axis" "Title of the graph"**

In the *test* directory, you can find some .txt files which *could* be useful to test out the code:
- **calibrated.txt** contains the data to perform FWHM vs. Amplifier gain linear fit [^1];
- **Sample2_90.txt** contains the data of a Compton measurement (for ϑ = 90°), to fit with a linear background [^2]; 
- other options are going to be added.

[^1]: To execute, type the following on the command line: ./compton calibrated.txt "Amplifier gain" "FWHM %" "FWHM vs. Amplifier gain"

[^2] To execute, type the following on the command line: ./compton Sample2_90.txt "ADC channel" "Counts" "Compton measurement"

 **Please note that this is still a work in progress! Any suggestions are welcome.**
