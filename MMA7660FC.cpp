// Author: Edoardo De Marchi
/* Copyright (C) 2012 mbed.org, MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 * and associated documentation files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 * BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
 
 
#include "MMA7660FC.h"

float TILT_XY[64] = {0, 2.69, 5.38, 8.08, 10.81, 13.55, 16.33, 19.16, 22.02, 24.95, 27.95, 31.04, 34.23, 37.54, 41.01, 44.68, 48.59, 52.83, 57.54, 62.95, 69.64, 79.86, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -79.86, -69.64, -62.95, -57.54, -52.83, -48.59, -44.68, -41.01, -37.54, -34.23, -31.04, -27.95, -24.95, -22.02, -19.16, -16.33, -13.55, -10.81, -8.08, -5.38, -2.69}; 
float TILT_Z[64] = {90.00, 87.31, 84.62, 81.92, 79.19, 76.45, 73.67, 70.84, 67.98, 65.05, 62.05, 58.96, 55.77, 52.46, 48.99, 45.32, 41.41, 37.17, 32.46, 27.05, 20.36, 10.14, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -10.14, -20.36, -27.05, -32.46, -37.17, -41.41, -45.32, -48.99, -52.46, -55.77, -58.96, -62.05, -65.05, -67.98, -70.84, -73.67, -76.45, -79.19, -81.92, -84.62, -87.31};


        // Connect module at I2C address addr using I2C port pins sda and scl
MMA7660FC::MMA7660FC(PinName sda, PinName scl, int addr) : m_i2c(sda, scl)
{
    SPI_W_Address = addr;                   // Write address
    SPI_R_Address = SPI_W_Address | 1;      // Read address                
}


        // Destroys instance
MMA7660FC::~MMA7660FC()
{
}


        // Device initialization
void MMA7660FC::init()
{ 
    //write_reg(INTSU_STATUS, 0x00);      // interrupt disable
    write_reg(SR_STATUS, 0X01);         // 64 Samples/Second   
}


        // Reads the tilt angle
void MMA7660FC::read_Tilt(float *x, float *y, float *z)
{
    const char Addr_X = OUT_X;
    char buf[3] = {0,0,0};
    
    m_i2c.write(SPI_W_Address, &Addr_X, 1);         // Pointer to the OUT_X register
    m_i2c.read(SPI_W_Address, buf, 3);              // Read register content into buffer with 6bit
    
    // returns the x, y, z coordinates transformed into full degrees
    *x = TILT_XY[(int)buf[0]];
    *y = TILT_XY[(int)buf[1]];
    *z = TILT_Z[(int)buf[2]];       
}


      // Reads x data
int MMA7660FC::read_x()
{
    m_i2c.start();                  // Start
    m_i2c.write(SPI_W_Address);     // A write to device
    m_i2c.write(OUT_X);             // Register to read
    m_i2c.start();                  
    m_i2c.write(SPI_R_Address);     // Read from device
    char x = m_i2c.read(0);         // Read the data
    m_i2c.stop();
    
    return (int)x;  
}


      // Reads y data
int MMA7660FC::read_y()
{
    m_i2c.start();                  // Start
    m_i2c.write(SPI_W_Address);     // A write to device
    m_i2c.write(OUT_Y);             // Register to read
    m_i2c.start();                  
    m_i2c.write(SPI_R_Address);     // Read from device
    char y = m_i2c.read(0);         // Read the data
    m_i2c.stop();
    
    return (int)y; 
}


      // Reads z data
int MMA7660FC::read_z()
{
    m_i2c.start();                  // Start
    m_i2c.write(SPI_W_Address);     // A write to device
    m_i2c.write(OUT_Z);             // Register to read
    m_i2c.start();                  
    m_i2c.write(SPI_R_Address);     // Read from device
    char z = m_i2c.read(0);         // Read the data
    m_i2c.stop();
    
    return (int)z;
}

    // Reads MMA7660FC Orientation
char const* MMA7660FC::read_Side()
{
    char tiltStatus = read_reg(TILT_STATUS);

    if((tiltStatus & 0x03) == 0x01)
        return "Front";
    if((tiltStatus & 0x03) == 0x02)
        return "Back";
    
    return "Unknown"; 
}


    // Reads MMA7660FC Orientation
char const* MMA7660FC::read_Orientation()
{
    char tiltStatus = read_reg(TILT_STATUS);

    if((tiltStatus & 0x1c) == 0x04)
        return "Left";
    if((tiltStatus & 0x1c) == 0x08)
        return "Right";
    if((tiltStatus & 0x1c) == 0x14)
        return "Down";
    if((tiltStatus & 0x1c) == 0x18)
        return "Up";
 
    return "Unknown";    
}



        // Read from specified MMA7660FC register
char MMA7660FC::read_reg(char addr)
{   
    m_i2c.start();                  // Start
    m_i2c.write(SPI_W_Address);     // A write to device
    m_i2c.write(addr);              // Register to read
    m_i2c.start();                  
    m_i2c.write(SPI_R_Address);     // Read from device
    char c = m_i2c.read(0);         // Read the data
    m_i2c.stop();                   
 
    return c;   
}


        // Write register (The device must be placed in Standby Mode to change the value of the registers) 
void MMA7660FC::write_reg(char addr, char data)
{
    char cmd[2] = {0, 0};
    
    cmd[0] = MODE_STATUS;
    cmd[1] = 0x00;                          // Standby Mode on
    m_i2c.write(SPI_W_Address, cmd, 2);
  
    cmd[0] = addr;
    cmd[1] = data;                          // New value of the register
    m_i2c.write(SPI_W_Address, cmd, 2); 
      
    cmd[0] = MODE_STATUS;
    cmd[1] = 0x01;                          // Active Mode on
    m_i2c.write(SPI_W_Address, cmd, 2);               
}


        //  check if the address exist on an I2C bus 
int MMA7660FC::check()
{ 
    return m_i2c.write(SPI_W_Address, NULL, 0); 
}