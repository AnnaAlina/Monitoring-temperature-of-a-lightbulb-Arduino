# Monitoring-temperature-of-a-lightbulb-Arduino
 
The purpose of this project is to demonstrate how an Arduino Uno can monitor the temperature from a light bulb source, and turn an ac sourced light bulb on or off depending on the input given. This project utilizes thermistors to sense the temperature and converts the change of resistance into a voltage to be analyzed. 

To begin, the the 5v from the arduino uno is utilized as the power source for the MOC3043M. The MOC3043M is a 6-pin zero-cross triac driver output optocoupler. This chip completely isolates the DC voltage from the AC voltage in the circuit. The DC side of the circuit includes the LCD display with a potentiometer, a thermistor voltage divider circuit, 3 LEDs that indicate temperature ranges, and an Arduino Uno. 

While a thermistor’s resistance varies with temperature, that value alone is not useful with the Arduino Uno, when connected in series with another resistor, that resistor value provides a reference point to calculate the varying voltage values at different temperatures. By connecting the center point of the voltage divider to an analog pin on the Uno, it can decipher the voltage reading with its Analog-to-Digital converter. By converting this 10-bit value to a real voltage value. This is accomplished with the following equation:

Where:

Rs = 4.7k   

ADC Value = Vi*1023/Vs

Rt =  thermistor resistance

Vs = 3.3v

Vi = measured value

As one can see, the voltage supplied to the thermistor is 3.3v. This differs from the rest of the DC circuit. The 3.3v pin of the Uno is tied to the AREF pin to create a reference voltage which increases the resolution of the measurement making it more sensitive. Five sample measurements are taken and averaged prior to outputting a true temperature value for even more precise accuracy.  To convert this value to temperature, the simplified B parameter equation was implemented. 

Where:

T = Temperature

To = Room Temp @ 25°C

B = Beta of thermistor = 3960

R = Resistance Measured

Ro = Resistance of thermistor @ room temp = 10k

1T = 1To+1Bln(RRo)

1T=125+13960ln(R10000)

Once a value has been calculated it can be inverted, and 273.15 will then be subtracted to produce a temperature in celsius. 

Part A utilized this temperature value to have the light remain on within the set parameters, which is to turn the light on continuously if the temperature is below 65 degrees and to turn it off if the temperature ever surpasses 80°C. Part B implements this temperature value in a linear proportional feedback control loop. This applies a corrective force that is linearly proportional to the amount of error. This is found with the formulas:

Where:

K = Gain Constant            

E = SP - PV

E = Error                

Output = E*K

SP = Set Point

PV = Process Variable

