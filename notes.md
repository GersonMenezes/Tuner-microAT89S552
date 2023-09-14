q =  C1	    32.70   30581.039755352 µs(p)       / 512 
w =  D1   	36.71   27240.533914465 µs(p)       / 512
e =  E1	    41.20   24271.844660194 µs(p)       / 512 
r =  F1	    43.65   22909.50744559 µs(p)        / 512
t =  G1	    49.00   20408.163265306 µs(p)        / 512 
y =  A1	    55.00   18181.818181818 µs(p)        / 512 
u =  B1	    61.74   16196.954972465 µs(p)        / 512 
i =  C2	    65.41   15288.182235132 µs(p)        / 512 
o =  D2	    73.42   13620.266957232 µs(p)        / 512 
p =  E2	    82.41   12134.449702706 µs(p)        / 512
a =  F2  	87.31   11453.441759249 µs(p)        / 512 
s =  G2  	98.00   10204.081632653 µs(p)        / 512 
d =  A2	    110.00  9090.9090909091 µs(p)        / 512 
f =  B2	    123.47  8099.133392727 µs(p)        / 512 
g =  C3	    130.81  7644.6754835257 µs(p)        / 512 
h =  D3	    146.83  6810.5972893823 µs(p)        / 512 
j =  E3	    164.81  6067.5929858625 µs(p)       / 512 
k =  F3	    174.61  5727.0488517267 µs(p)       / 512 
l =  G3  	196.00  5102.0408163265 µs(p)       / 512 
ç =  A3	    220.00  4545.4545454545 µs(p)       / 512 
z =  B3  	246.94  4049.5666963635 µs(p)       / 512
x =  C4  	261.63  3822.1916446891 µs(p)       / 512 
c =  D4	    293.66  3405.2986446911 µs(p)       / 512 
v =  E4	    329.63  3033.7044565118 µs(p)       / 512 
b =  F4	    349.23  2863.4424304899 µs(p)       / 512 
n =  G4	    392.00  2551.0204081633 µs(p)        / 512 
m =  A4  	440.00  2272.7272727273 µs(p)       / 512 
, =  B4  	493.88  2024.7833481817 µs(p)       / 512 = 
. =  C5	    523.25  1911.132345915 µs(p)        / 512 = 3.73

HZ to us
us = 10⁶ / HZ
us / 512 = period of it point
## Senoidal
- Duração total 1 segundo
- período de cada seno vai de 0 a 255 e de volta para 0
- Menor período 1 / 440 hz = 
[]
1s/2ms
2ms
tmp = 2000

0 delay(tempo) 1 2 3 



	if (state == 1)
	{ // Piano mode

		if (char_temp == 27)
		{

			reset();
		}

		play_note(char_temp, 1000);
	}
	else if (state == 2)
	{ // Composer mode

		if (char_temp == 27)
		{

			reset();
		}

		/*else if (countNotes == 40 || char_temp == 13)
		{

			playComposer();

			countNotes = 0;

			state = 3;
		}*/
		else
		{
			notes[countNotes] = char_temp;

			countNotes++;
		}
	}
	else
	{ // Menu mode

		if (char_temp == '1')
		{

			aponta_line1();

			write_lcd_string(MSG4);

			aponta_line2();

			write_lcd_string(MSG5);

			state = 1;
		}
		else if (char_temp == '2')
		{

			aponta_line1();

			write_lcd_string(MSG6);

			aponta_line2();

			write_lcd_string(MSG7);

			aponta_line1();

			write_lcd_string(MSG6);

			state = 2;
		}else{
			aponta_line1();

			write_lcd_string(MSG6);

			aponta_line2();

			write_lcd_string(MSG7);

			state = 2;
		}
	}