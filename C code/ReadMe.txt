How to run program.

1. Setup:
Ensure your DE10-Nano is set up with Linux, connected to your computer, and accessible over the network. If not, follow the tutorials to set up Linux on FPGA and establish a connection between FPGA and your Windows computer.

2. File Transfer:
Download done.zip to your desktop. Open a command prompt on your computer and use scp to transfer the file to the DE10-Nano.
scp [Path_To_Desktop]\done.zip root@de10-nano: root@de10-nano:~/Desktop


3. switch to your VNC veiwer - Unzipping:
Run the following commands in the DE10-Nano terminal to unzip done.zip and change the current directory to the done directory:
open termainal on de10 nano and run unzip done.zip.
unzip ~/Desktop/done.zip -d ~/Desktop
cd ~/Desktop/done

4. Grant Permission:
Run the following command to grant execute permission to the edge program:
chmod +x edge

5. Run the Program:
Run the edge program with the desired input and output BMP files:
./edge input1.bmp output1.bmp
./edge input2.bmp output2.bmp
./edge input3.bmp output3.bmp
./edge input4.bmp output4.bmp
./edge input5.bmp output5.bmp


6. Using Your Images:
If you want to use your images, ensure they are 24-bit BMP files, transfer them to DE10-Nano using scp, and run the edge program with your file names.
./edge your_file_name.bmp output_name.bmp


To convert other into bmps
convert input_name.png -type TrueColor output.bmp
convert input_name.jpg -type TrueColor output.bmp




