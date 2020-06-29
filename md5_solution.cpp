// Example program
#include <iostream>
#include <string> 
#include <bitset>
using namespace std;

// Divide the message in parts of 32 bit and make little endians
void part(string *s, bool *message, int i){
	char d [32];
	char h1 [8], h2[8], h3[8], h4[8];
	int help = 0;
	for (int j = 0; j<8; j++){
		h1 [j] = message[32*(i-1)+help] ?  '1' : '0';
		help++;
	}
	for (int j = 0; j<8; j++){
		h2 [j] = message[32*(i-1)+help] ?  '1' : '0';
		help++;
	}
	for (int j = 0; j<8; j++){
		h3 [j] = message[32*(i-1)+help] ?  '1' : '0';
		help++;
	}
	for (int j = 0; j<8; j++){
		h4 [j] = message[32*(i-1)+help] ?  '1' : '0';
		help++;
	}
	help = 0;
	for (int j = 0; j < 8; j++){
		d[help] = h4[j];
		help++;
	}
	for (int j = 0; j < 8; j++){
		d[help] = h3[j];
		help++;
	}
	for (int j = 0; j < 8; j++){
		d[help] = h2[j];
		help++;
	}
	for (int j = 0; j < 8; j++){
		d[help] = h1[j];
		help++;
	}
	s[0] = d;
}

// left rotate
unsigned int left_rotate(unsigned int x, unsigned int c) {
            return (x << c) | (x >> (32 - c));
        }
        
// funktion F
unsigned int F (unsigned int X, unsigned int Y, unsigned int Z){
	return X&Y|~X&Z;	
}

// funktion G
unsigned int G (unsigned int X, unsigned int Y, unsigned int Z){
	return Z&X|~Z&Y;	
}

// funktion H
unsigned int H (unsigned int X, unsigned int Y, unsigned int Z){
	return X^Y^Z;	
}

// funktion I
unsigned int I (unsigned int X, unsigned int Y, unsigned int Z){
	return Y^(X|~Z);	
}

int main(){
	
  	// Initialisierung 	
  	string s = "Kim";
  	cout << "String to encrypt: " << s << "\n\n";
  
  	unsigned int s_i[64] = { 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
  							5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20, 
							4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
							6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21 };
  	unsigned int k[64] ={ 0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee, 0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501, 
						  	0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be, 0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821, 
	 						0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa, 0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
							0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed, 0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
							0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c, 0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
							0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05, 0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
							0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039, 0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
							0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1, 0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391 };
	unsigned int a_0 = 0x67452301;
  	unsigned int b_0 = 0xefcdab89;
  	unsigned int c_0 = 0x98badcfe;
  	unsigned int d_0 = 0x10325476;
  	
  	// Processing of the input:
	int n = s.length();
	int laenge = 8*n;
	bool input[laenge];
	for(int i = 0; i < laenge; i++){
    	input[i] = 0;
	}
  	int help = 0;
	for (int i = 0; i < n; i++){
  		bitset<8> letter(s[i]);
  		for (int j = 7; j >= 0; j--){
  			input[help] = letter[j];
  			help++;
	  	}
  	}
  	
	// calculating the necessary length
  	n = 512*(laenge/512)+448;
  	bool message [n+64];
  	
  	// append zeros
  	for (int i = 0; i < (n+64); i++){
  		message [i] = 0;
  	} 
  	for (int i = 0; i < laenge ; i++){
  		message [i] = input[i];
 	}
 	// append one bit to input ( change one 0 bit to 1)
  	message [laenge] = 1;
  	// apppend the length of the input
  	bitset<64> l(laenge);
  	help = 63;
  	for (int i = n; i < (n+64); i++){
  		message[i] = l[help];
  		help--;
  	}  
    
	// calculate the number of 512 bit blocks
	int x = ((n+64)/512);

	// For every 512 block
    for (int w = 0; w < x; w++){
		// Get the 512 bit long block
		bool message_x [512];
		for (int i = 0; i < 512; i++){
			message_x[i] = message[w*512 + i];
		}
		
		// Get the 16 32-bit long values
	  	string *m_1 = new string();
	  	string *m_2 = new string();
	  	string *m_3 = new string();
	  	string *m_4 = new string();
	  	string *m_5 = new string();
	  	string *m_6 = new string();
	  	string *m_7 = new string();
	  	string *m_8 = new string();
	  	string *m_9 = new string();
	  	string *m_10 = new string();
	  	string *m_11 = new string();
	  	string *m_12 = new string();
	  	string *m_13 = new string();
	  	string *m_14 = new string();
	  	string *m_15 = new string();
	 	string *m_16 = new string();
	  	part(m_1, message_x, 1);
	  	part(m_2, message_x, 2);
	  	part(m_3, message_x, 3);
	  	part(m_4, message_x, 4);
	  	part(m_5, message_x, 5);
	  	part(m_6, message_x, 6);
	  	part(m_7, message_x, 7);
	  	part(m_8, message_x, 8);
	  	part(m_9, message_x, 9);
	  	part(m_10, message_x, 10);
	  	part(m_11, message_x, 11);
	  	part(m_12, message_x, 12);
	  	part(m_13, message_x, 13);
	  	part(m_14, message_x, 14);
	  	part(m_15, message_x, 15);
	  	part(m_16, message_x, 16);
	
	  	bitset<32> m_1_bit(*m_1);
	  	bitset<32> m_2_bit(*m_2);
	  	bitset<32> m_3_bit(*m_3);
	  	bitset<32> m_4_bit(*m_4);
	  	bitset<32> m_5_bit(*m_5);
	  	bitset<32> m_6_bit(*m_6);
	  	bitset<32> m_7_bit(*m_7);
	  	bitset<32> m_8_bit(*m_8);
	  	bitset<32> m_9_bit(*m_9);
	  	bitset<32> m_10_bit(*m_10);
	  	bitset<32> m_11_bit(*m_11);
	  	bitset<32> m_12_bit(*m_12);
	  	bitset<32> m_13_bit(*m_13);
	  	bitset<32> m_14_bit(*m_14);
	  	bitset<32> m_15_bit(*m_15);
	  	bitset<32> m_16_bit(*m_16);
	
	  	unsigned int m1 = m_1_bit.to_ulong();
	  	unsigned int m2 = m_2_bit.to_ulong();
	  	unsigned int m3 = m_3_bit.to_ulong();
	  	unsigned int m4 = m_4_bit.to_ulong();
	  	unsigned int m5 = m_5_bit.to_ulong();
	  	unsigned int m6 = m_6_bit.to_ulong();
	  	unsigned int m7 = m_7_bit.to_ulong();
	  	unsigned int m8 = m_8_bit.to_ulong();
	  	unsigned int m9 = m_9_bit.to_ulong();
	  	unsigned int m10 = m_10_bit.to_ulong();
	  	unsigned int m11 = m_11_bit.to_ulong();
	  	unsigned int m12 = m_12_bit.to_ulong();
	  	unsigned int m13 = m_13_bit.to_ulong();
	  	unsigned int m14 = m_14_bit.to_ulong();
	  	unsigned int m15 = m_15_bit.to_ulong();
	  	unsigned int m16 = m_16_bit.to_ulong();
	  	
	  	if (w+1 == x){
	  		m16 = (((m16>>24) & 0x000000ff) | ((m16>>8) & 0x0000ff00) | ((m16<<8) & 0x00ff0000) | ((m16<<24) & 0xff000000));
	    	m15 = (((m15>>24) & 0x000000ff) | ((m15>>8) & 0x0000ff00) | ((m15<<8) & 0x00ff0000) | ((m15<<24) & 0xff000000));
	    	unsigned int tempo = m16;
	    	m16 = m15;
	    	m15 = tempo;
		}
	  	unsigned int m[16] = {m1,m2,m3,m4,m5,m6,m7,m8,m9,m10,m11,m12,m13,m14,m15,m16};
		
	  	unsigned int a = a_0;
	  	unsigned int b = b_0;
	  	unsigned int c = c_0;
	  	unsigned int d = d_0;
	  
	  	for (int i = 0; i < 64; i++){
	  		unsigned int f, g;
	  		if (0 <= i && i <= 15){
	  			f = F(b,c,d);
	  			g = i;
			}
			if(16 <= i && i <= 31){
				f = G(b,c,d);
				g = (5*i + 1) % 16;
			}
			if(32 <= i && i <= 47){
				f = H(b,c,d);
				g = (3*i + 5) % 16;
			}
			if(48 <= i && i <= 63){
				f = I(b,c,d);
				g = (7*i) % 16;
			}
			
			f = f + a + k[i] + m[g];
			a = d;
			d = c;
			c = b;
			b = b + left_rotate(f,s_i[i]);
		}
	  	a_0 = a_0 + a;
	  	b_0 = b_0 + b;
	  	c_0 = c_0 + c;
	  	d_0 = d_0 + d;	
	}
	// convert endians
	a_0 = (((a_0>>24) & 0x000000ff) | ((a_0>>8) & 0x0000ff00) | ((a_0<<8) & 0x00ff0000) | ((a_0<<24) & 0xff000000));
	b_0 = (((b_0>>24) & 0x000000ff) | ((b_0>>8) & 0x0000ff00) | ((b_0<<8) & 0x00ff0000) | ((b_0<<24) & 0xff000000));
	c_0 = (((c_0>>24) & 0x000000ff) | ((c_0>>8) & 0x0000ff00) | ((c_0<<8) & 0x00ff0000) | ((c_0<<24) & 0xff000000));
	d_0 = (((d_0>>24) & 0x000000ff) | ((d_0>>8) & 0x0000ff00) | ((d_0<<8) & 0x00ff0000) | ((d_0<<24) & 0xff000000));
	
	// convert to hex values
	char a_hex[8], b_hex[8], c_hex[8], d_hex[8];
   	sprintf(a_hex, "%X", a_0);
   	sprintf(b_hex, "%X", b_0);
   	sprintf(c_hex, "%X", c_0);
   	sprintf(d_hex, "%X", d_0);

	// output
  	cout << "Hash:"<< a_hex << b_hex << c_hex << d_hex;

  	return 0;
  
}
