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
 
 
#ifndef MBED_MMA7660FC_H
#define MBED_MMA7660FC_H
 
#include "mbed.h"

#define ADDR_MMA7660 0x98                   // I2C SLAVE ADDR MMA7660FC
#define OUT_X 0x00              // [6:0] are Read Only 6-bit output value X (XOUT[5] is 0 if the g direction is positive, 1 is negative)
#define OUT_Y 0x01              // [6:0] are Read Only 6-bit output value Y (YOUT[5] is 0 if the g direction is positive, 1 is negative)
#define OUT_Z 0x02              // [6:0] are Read Only 6-bit output value Z (ZOUT[5] is 0 if the g direction is positive, 1 is negative)
#define TILT_STATUS 0x03        // Tilt Status (Read only)
#define SRST_STATUS 0x04        // Sample Rate Status Register (Read only)
#define SPCNT_STATUS 0x05       // Sleep Count Register (Read/Write)
#define INTSU_STATUS 0x06       // Interrupt Setup Register
#define MODE_STATUS 0x07        // Mode Register (Read/Write)
#define SR_STATUS 0x08          // Auto-Wake and Active Mode Portrait/Landscape Samples per Seconds Register (Read/Write)
#define PDET_STATUS 0x09        // Tap/Pulse Detection Register (Read/Write)
#define PD_STATUS 0xA           // Tap/Pulse Debounce Count Register (Read/Write)




 
/** Accelerometer MMA7660FC class 
 *
 * Example:
 * @code
 * 
 * #include "mbed.h"
 * #include "MMA7660FC.h"
 * 
 * #define ADDR_MMA7660 0x98
 * 
 * MMA7660FC Acc(p28, p27, ADDR_MMA7660);
 * serial pc(USBTX, USBRX);
 *
 * int main() 
 * {
 *   Acc.init(); 
 *      while(1)
 *      {
 *          int x=0, y=0, z=0;
 *          Acc.read_Tilt(&x, &y, &z);
 *          pc.printf("Tilt x: %2.2f degree \n", x);                    // Print the tilt orientation of the X axis
 *          pc.printf("Tilt y: %2.2f degree \n", y);                    // Print the tilt orientation of the Y axis
 *          pc.printf("Tilt z: %2.2f degree \n", z);                    // Print the tilt orientation of the Z axis
 *          wait(1);       
 *      }
 * }
 * @endcode
 */ 
class MMA7660FC         
{        
    public:
        
        
       /** Creates an MMA7660FC object connected to the specified I2C object
        *
        * @param sda I2C data port
        * @param scl I2C clock port
        * @param addr The address of the MMA7660FC
        */ 
      MMA7660FC(PinName sda, PinName scl, int addr);
       
       /** Destroys an MMA7660FC object
        *
        */
      ~MMA7660FC();
      
       /** Initialization of device MMA7660FC (required)
        *
        */
      void init();
    
       /** Read the Tilt Angle using Three Axis
        *
        * @param *x Value of x tilt
        * @param *y Value of y tilt
        * @param *z Value of z tilt
        */
      void read_Tilt(float *x, float *y, float *z);
      
      /** Reads the x register of the MMA7660FC
        *
        * @returns The value of x acceleration
        */
      int read_x();
      
      /** Reads the y register of the MMA7660FC
        *
        * @returns The value of y acceleration
        */
      int read_y();
      
      /** Reads the z register of the MMA7660FC
        *
        * @returns The value of z acceleration
        */
       int read_z();
       
      /** Reads the Front or Back position of the device
        *
        * @returns The Front or Back position
        */       
       char const* read_Side();
       
      /** Reads the Orientation of the device
        *
        * @returns The Left or Right or Down or Up Orientation
        */       
       char const* read_Orientation();
            
        /** Reads from specified MMA7660FC register
         *
         * @param addr The internal registeraddress of the MMA7660FC
         * @returns The value of the register
         */
      char read_reg(char addr);
        
        /** Writes to specified MMA7660FC register
        *
        * @param addr The internal registeraddress of the MMA7660FC
        * @param data New value of the register
        */    
      void write_reg(char addr, char data); 
      
        /** Checks if the address exist on an I2C bus 
        *
        * @returns 0 on success, or non-0 on failure
        */    
      int check();
      
   
    private:
      I2C m_i2c;
      //int m_addr;   
      char SPI_R_Address;
      char SPI_W_Address; 
};

#endif 