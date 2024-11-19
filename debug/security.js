/*
 * Simple Virtual Machine - A versatile and robust architecture to
 * easily write applications.
 * Copyright (C) 2021  Julien BRUGUIER
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * This SHA-1 implementation is not mine. I took it on the www.
 * Thanks to his author to have shared it.
 */

/*
 * A JavaScript implementation of the Secure Hash Algorithm, SHA-1, as defined
 * in FIPS 180-1
 * Version 2.2 Copyright Paul Johnston 2000 - 2009.
 * Other contributors: Greg Holt, Andrew Kepert, Ydnar, Lostinet
 * Distributed under the BSD License
 * See http://pajhome.org.uk/crypt/md5 for details.
 */

/*
 * Configurable variables. You may need to tweak these to be compatible with
 * the server-side, but the defaults work in most cases.
 */
var hexcase = 0;  /* hex output format. 0 - lowercase; 1 - uppercase        */
var b64pad  = ""; /* base-64 pad character. "=" for strict RFC compliance   */

/*
 * These are the functions you'll usually want to call
 * They take string arguments and return either hex or base-64 encoded strings
 */
function raw_sha1(s)    { return rstr_sha1(str2rstr_utf8(s)); }
function hex_sha1(s)    { return rstr2hex(rstr_sha1(str2rstr_utf8(s))); }
function b64_sha1(s)    { return rstr2b64(rstr_sha1(str2rstr_utf8(s))); }
function any_sha1(s, e) { return rstr2any(rstr_sha1(str2rstr_utf8(s)), e); }
function hex_hmac_sha1(k, d) { return rstr2hex(rstr_hmac_sha1(str2rstr_utf8(k), str2rstr_utf8(d))); }
function b64_hmac_sha1(k, d) { return rstr2b64(rstr_hmac_sha1(str2rstr_utf8(k), str2rstr_utf8(d))); }
function any_hmac_sha1(k, d, e) { return rstr2any(rstr_hmac_sha1(str2rstr_utf8(k), str2rstr_utf8(d)), e); }

/*
 * Perform a simple self-test to see if the VM is working
 */
function sha1_vm_test()
{
	return hex_sha1("abc").toLowerCase() == "a9993e364706816aba3e25717850c26c9cd0d89d";
}

/*
 * Calculate the SHA1 of a raw string
 */
function rstr_sha1(s)
{
	return binb2rstr(binb_sha1(rstr2binb(s), s.length * 8));
}

/*
 * Calculate the HMAC-SHA1 of a key and some data (raw strings)
 */
function rstr_hmac_sha1(key, data)
{
	var bkey = rstr2binb(key);
	if(bkey.length > 16) bkey = binb_sha1(bkey, key.length * 8);

	var ipad = Array(16), opad = Array(16);
	for(var i = 0; i < 16; i++)
	{
		ipad[i] = bkey[i] ^ 0x36363636;
		opad[i] = bkey[i] ^ 0x5C5C5C5C;
	}

	var hash = binb_sha1(ipad.concat(rstr2binb(data)), 512 + data.length * 8);
	return binb2rstr(binb_sha1(opad.concat(hash), 512 + 160));
}

/*
 * Convert a raw string to a hex string
 */
function rstr2hex(input)
{
	try { hexcase } catch(e) { hexcase=0; }
	var hex_tab = hexcase ? "0123456789ABCDEF" : "0123456789abcdef";
	var output = "";
	var x;
	for(var i = 0; i < input.length; i++)
	{
		x = input.charCodeAt(i);
		output += hex_tab.charAt((x >>> 4) & 0x0F)
			+  hex_tab.charAt( x        & 0x0F);
	}
	return output;
}

function hex2rstr(input)
{
	var output = '';
	var hex_tab = "0123456789abcdef";
	for(var i = 0; i < input.length; i+=2)
	{
		output += String.fromCharCode((hex_tab.indexOf(input.charAt(i)) << 4) + (hex_tab.indexOf(input.charAt(i+1))));
	}
	return output;
}

/*
 * Convert a raw string to a base-64 string
 */
function rstr2b64(input)
{
	try { b64pad } catch(e) { b64pad=''; }
	var tab = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	var output = "";
	var len = input.length;
	for(var i = 0; i < len; i += 3)
	{
		var triplet = (input.charCodeAt(i) << 16)
			| (i + 1 < len ? input.charCodeAt(i+1) << 8 : 0)
			| (i + 2 < len ? input.charCodeAt(i+2)      : 0);
		for(var j = 0; j < 4; j++)
		{
			if(i * 8 + j * 6 > input.length * 8) output += b64pad;
			else output += tab.charAt((triplet >>> 6*(3-j)) & 0x3F);
		}
	}
	return output;
}

/*
 * Convert a raw string to an arbitrary string encoding
 */
function rstr2any(input, encoding)
{
	var divisor = encoding.length;
	var remainders = Array();
	var i, q, x, quotient;

	/* Convert to an array of 16-bit big-endian values, forming the dividend */
	var dividend = Array(Math.ceil(input.length / 2));
	for(i = 0; i < dividend.length; i++)
	{
		dividend[i] = (input.charCodeAt(i * 2) << 8) | input.charCodeAt(i * 2 + 1);
	}

	/*
	 * Repeatedly perform a long division. The binary array forms the dividend,
	 * the length of the encoding is the divisor. Once computed, the quotient
	 * forms the dividend for the next step. We stop when the dividend is zero.
	 * All remainders are stored for later use.
	 */
	while(dividend.length > 0)
	{
		quotient = Array();
		x = 0;
		for(i = 0; i < dividend.length; i++)
		{
			x = (x << 16) + dividend[i];
			q = Math.floor(x / divisor);
			x -= q * divisor;
			if(quotient.length > 0 || q > 0)
				quotient[quotient.length] = q;
		}
		remainders[remainders.length] = x;
		dividend = quotient;
	}

	/* Convert the remainders to the output string */
	var output = "";
	for(i = remainders.length - 1; i >= 0; i--)
		output += encoding.charAt(remainders[i]);

	/* Append leading zero equivalents */
	var full_length = Math.ceil(input.length * 8 /
			(Math.log(encoding.length) / Math.log(2)))
		for(i = output.length; i < full_length; i++)
			output = encoding[0] + output;

	return output;
}

/*
 * Encode a string as utf-8.
 * For efficiency, this assumes the input is valid utf-16.
 */
function str2rstr_utf8(input)
{
	var output = "";
	var i = -1;
	var x, y;

	while(++i < input.length)
	{
		/* Decode utf-16 surrogate pairs */
		x = input.charCodeAt(i);
		y = i + 1 < input.length ? input.charCodeAt(i + 1) : 0;
		if(0xD800 <= x && x <= 0xDBFF && 0xDC00 <= y && y <= 0xDFFF)
		{
			x = 0x10000 + ((x & 0x03FF) << 10) + (y & 0x03FF);
			i++;
		}

		/* Encode output as utf-8 */
		if(x <= 0x7F)
			output += String.fromCharCode(x);
		else if(x <= 0x7FF)
			output += String.fromCharCode(0xC0 | ((x >>> 6 ) & 0x1F),
					0x80 | ( x         & 0x3F));
		else if(x <= 0xFFFF)
			output += String.fromCharCode(0xE0 | ((x >>> 12) & 0x0F),
					0x80 | ((x >>> 6 ) & 0x3F),
					0x80 | ( x         & 0x3F));
		else if(x <= 0x1FFFFF)
			output += String.fromCharCode(0xF0 | ((x >>> 18) & 0x07),
					0x80 | ((x >>> 12) & 0x3F),
					0x80 | ((x >>> 6 ) & 0x3F),
					0x80 | ( x         & 0x3F));
	}
	return output;
}

/*
 * Encode a string as utf-16
 */
function str2rstr_utf16le(input)
{
	var output = "";
	for(var i = 0; i < input.length; i++)
		output += String.fromCharCode( input.charCodeAt(i)        & 0xFF,
				(input.charCodeAt(i) >>> 8) & 0xFF);
	return output;
}

function str2rstr_utf16be(input)
{
	var output = "";
	for(var i = 0; i < input.length; i++)
		output += String.fromCharCode((input.charCodeAt(i) >>> 8) & 0xFF,
				input.charCodeAt(i)        & 0xFF);
	return output;
}

/*
 * Convert a raw string to an array of big-endian words
 * Characters >255 have their high-byte silently ignored.
 */
function rstr2binb(input)
{
	var output = Array(input.length >> 2);
	for(var i = 0; i < output.length; i++)
		output[i] = 0;
	for(var i = 0; i < input.length * 8; i += 8)
		output[i>>5] |= (input.charCodeAt(i / 8) & 0xFF) << (24 - i % 32);
	return output;
}

/*
 * Convert an array of big-endian words to a string
 */
function binb2rstr(input)
{
	var output = "";
	for(var i = 0; i < input.length * 32; i += 8)
		output += String.fromCharCode((input[i>>5] >>> (24 - i % 32)) & 0xFF);
	return output;
}

/*
 * Calculate the SHA-1 of an array of big-endian words, and a bit length
 */
function binb_sha1(x, len)
{
	/* append padding */
	x[len >> 5] |= 0x80 << (24 - len % 32);
	x[((len + 64 >> 9) << 4) + 15] = len;

	var svm_window = Array(80);
	var a =  1732584193;
	var b = -271733879;
	var c = -1732584194;
	var d =  271733878;
	var e = -1009589776;

	for(var i = 0; i < x.length; i += 16)
	{
		var olda = a;
		var oldb = b;
		var oldc = c;
		var oldd = d;
		var olde = e;

		for(var j = 0; j < 80; j++)
		{
			if(j < 16) svm_window[j] = x[i + j];
			else svm_window[j] = bit_rol(svm_window[j-3] ^ svm_window[j-8] ^ svm_window[j-14] ^ svm_window[j-16], 1);
			var t = safe_add(safe_add(bit_rol(a, 5), sha1_ft(j, b, c, d)),
					safe_add(safe_add(e, svm_window[j]), sha1_kt(j)));
			e = d;
			d = c;
			c = bit_rol(b, 30);
			b = a;
			a = t;
		}

		a = safe_add(a, olda);
		b = safe_add(b, oldb);
		c = safe_add(c, oldc);
		d = safe_add(d, oldd);
		e = safe_add(e, olde);
	}
	return Array(a, b, c, d, e);

}

/*
 * Perform the appropriate triplet combination function for the current
 * iteration
 */
function sha1_ft(t, b, c, d)
{
	if(t < 20) return (b & c) | ((~b) & d);
	if(t < 40) return b ^ c ^ d;
	if(t < 60) return (b & c) | (b & d) | (c & d);
	return b ^ c ^ d;
}

/*
 * Determine the appropriate additive constant for the current iteration
 */
function sha1_kt(t)
{
	return (t < 20) ?  1518500249 : (t < 40) ?  1859775393 :
		(t < 60) ? -1894007588 : -899497514;
}

/*
 * Add integers, wrapping at 2^32. This uses 16-bit operations internally
 * to work around bugs in some JS interpreters.
 */
function safe_add(x, y)
{
	var lsw = (x & 0xFFFF) + (y & 0xFFFF);
	var msw = (x >> 16) + (y >> 16) + (lsw >> 16);
	return (msw << 16) | (lsw & 0xFFFF);
}

/*
 * Bitwise rotate a 32-bit number to the left.
 */
function bit_rol(num, cnt)
{
	return (num << cnt) | (num >>> (32 - cnt));
}

function svm_derivated_keys(svm_main_key, svm_challenges)
{
	var cs = [];
	for(var i=0 ; i<svm_challenges.length ; ++i)
	{
		cs[i] = raw_sha1(svm_main_key+svm_challenges[i][0]) + raw_sha1(svm_main_key+svm_challenges[i][1]);
	}
	return cs;
}

function svm_circular_shift_16(d,v)
{
	return ((v << d) & 0xFFFF) | (v >>> (16-d));
}

function svm_circular_shift_8(d,v)
{
	return ((v << d) & 0xFF) | (v >>> (8-d));
}

function svm_transpose(m,t)
{
	for(var l=0 ; l<t ; ++l)
	{
		for(var c=(l+1) ; c<t ; ++c)
		{
			var svm_normal_byte_index = l*Math.floor(t/8) + Math.floor(c/8);
			var svm_normal_bit_index = 7-c%8;
			var svm_normal_bit = (m[svm_normal_byte_index] >>> svm_normal_bit_index) & 1;
			var svm_mirror_byte_index = c*Math.floor(t/8) + Math.floor(l/8);
			var svm_mirror_bit_index = 7-l%8;
			var svm_mirror_bit = (m[svm_mirror_byte_index] >>> svm_mirror_bit_index) & 1;
			if(svm_normal_bit)
			{
				m[svm_mirror_byte_index] |= 1 << svm_mirror_bit_index;
			}
			else
			{
				m[svm_mirror_byte_index] &= ~ (1 << svm_mirror_bit_index);
			}
			if(svm_mirror_bit)
			{
				m[svm_normal_byte_index] |= 1 << svm_normal_bit_index;
			}
			else
			{
				m[svm_normal_byte_index] &= ~ (1 << svm_normal_bit_index);
			}
		}
	}
}

function svm_apply_mask(m,s,c)
{
	for(var j=0 ; j<32 ; j+=8)
	{
		for(var k=0 ; k<8 ; ++k)
		{
			var d=(((c[32]>>>k)&1)<<2) + (((c[33]>>>k)&1)<<1) + (((c[34]>>>k)&1));
			s[k]=svm_circular_shift_8(d,s[k]);
		}
		svm_transpose(s,8);
		for(var k=0 ; k<8 ; ++k)
		{
			var d=(((c[35]>>>k)&1)<<2) + (((c[36]>>>k)&1)<<1) + (((c[37]>>>k)&1));
			s[k]=svm_circular_shift_8(d,s[k]);
		}
		svm_transpose(s,8);
		var a=c[38];
		var b=c[39];
		for(var k=0 ; k<8 ; k+=2)
		{
			s[k] ^=a;
			s[k+1] ^=b;
			var v=((a&0xFF)<<8) + (b&0xFF);
			v = svm_circular_shift_16(4,v);
			a = v>>>8;
			b = v & 0xFF;
		}
		for(var k=0 ; k<8 ; ++k)
		{
			m[j+k] ^= s[k];
		}
	}
}

function svm_cipher(message, svm_derivated_keys)
{
	var mesg = message.split('').map(function(e) { return e.charCodeAt(0); });
	for(var c=0 ; c<svm_derivated_keys.length ; ++c)
	{
		var svm_key = svm_derivated_keys[c].split('').map(function(e) { return e.charCodeAt(0); });
		var svm_round = [];
		var svm_padding = false;
		var svm_shift = [];
		for(var j=0 ; j<4 ; ++j)
		{
			svm_shift[3-j] = (svm_key[16+2*j] << 8) | svm_key[16+2*j+1];
		}
		var svm_mask = [];
		var svm_padding_size = 0;
		for(var i=0 ; i<8 ; ++i)
		{
			svm_mask[i] = svm_key[24+i];
		}
		for(var i=0 ; ; i+=32)
		{
			var svm_matrix = [];
			var svm_end = false;
			for(var j=0 ; j<32 ; ++j)
			{
				if((i+j)<mesg.length)
				{
					svm_matrix[j]=mesg[i+j];
				}
				else
				{
					svm_matrix[j]=Math.floor(Math.random()*256);
					if(!svm_padding)
					{
						svm_padding_size = 32-j;
						svm_padding = true;
						if(j<27)
						{
							svm_end = true;
						}
					}
					else
					{
						if(j==0)
						{
							svm_end = true;
						}
					}
					if(svm_end && (j>=27))
					{
						var bit=(svm_padding_size >>> (31-j)) & 1;
						var svm_parity = 0;
						var element = svm_matrix[j];
						while(element>0)
						{
							svm_parity ^= (element&1);
							element>>>=1;
						}
						if(bit ^ svm_parity)
						{
							svm_matrix[j] ^= (1 << (Math.floor(Math.random()*8)));
						}
					}
				}
			}
			for(var j=0 ; j<16 ; ++j)
			{
				var cc = svm_key[j] & 0xF;
				var svm_line = (svm_matrix[2*j] << 8) + (svm_matrix[2*j+1]);
				var svm_shifted_line = svm_circular_shift_16(cc,svm_line);
				svm_matrix[2*j] = svm_shifted_line >>> 8;
				svm_matrix[2*j+1] = svm_shifted_line & 0xFF;
			}
			svm_transpose(svm_matrix,16);
			for(var j=0 ; j<16 ; ++j)
			{
				var cc = (svm_key[j]>>>4) & 0xF;
				var svm_line = (svm_matrix[2*j] << 8) + (svm_matrix[2*j+1]);
				var svm_shifted_line = svm_circular_shift_16(cc,svm_line);
				svm_matrix[2*j] = svm_shifted_line >>> 8;
				svm_matrix[2*j+1] = svm_shifted_line & 0xFF;
			}
			svm_transpose(svm_matrix,16);
			for(var j=0 ; j<32 ; j+=8)
			{
				var svm_value = [];
				for(var k=0 ; k<4 ; ++k)
				{
					svm_value[3-k] = (svm_matrix[j+2*k] <<8) + (svm_matrix[j+2*k+1]);
				}
				var r = 0;
				for(var k=0 ; k<4 ; ++k)
				{
					var s = svm_value[k]+svm_shift[k]+r;
					svm_value[k] = s & 0xFFFF;
					r=s >>> 16;
				}
				for(var k=0 ; k<4 ; ++k)
				{
					svm_matrix[j+2*k]=svm_value[k] & 0xFF;
					svm_matrix[j+2*k+1]=svm_value[k] >>> 8;
				}
			}
			svm_apply_mask(svm_matrix,svm_mask,svm_key);
			svm_round = svm_round.concat(svm_matrix);
			if(svm_end)
			{
				break;
			}
		}
		mesg = svm_round;
	}
	return mesg.map(function(e) { return String.fromCharCode(e); }).join('');
}

function svm_decipher(message, svm_derivated_keys)
{
	var mesg = message.split('').map(function(e) { return e.charCodeAt(0); });
	for(var c=svm_derivated_keys.length-1 ; c>=0 ; --c)
	{
		var svm_key = svm_derivated_keys[c].split('').map(function(e) { return e.charCodeAt(0); });
		var svm_round = [];
		var svm_shift = [];
		for(var j=0 ; j<4 ; ++j)
		{
			svm_shift[3-j] = (svm_key[16+2*j] << 8) | svm_key[16+2*j+1];
		}
		var svm_mask = [];
		var svm_padding_size = 0;
		for(var i=0 ; i<8 ; ++i)
		{
			svm_mask[i] = svm_key[24+i];
		}
		for(var i=0 ; i<mesg.length ; i+=32)
		{
			var svm_matrix = [];
			for(var j=0 ; j<32 ; ++j)
			{
				if((i+j)>=mesg.length)
				{
					return '';
				}
				svm_matrix[j] = mesg[i+j];
			}
			svm_apply_mask(svm_matrix,svm_mask,svm_key);
			for(var j=0 ; j<32 ; j+=8)
			{
				var svm_value = [];
				for(var k=0 ; k<4 ; ++k)
				{
					svm_value[3-k] = (svm_matrix[j+7-2*k] <<8) + (svm_matrix[j+7-(2*k+1)]);
				}
				var r = 0;
				for(var k=0 ; k<4 ; ++k)
				{
					var s = svm_value[k]-svm_shift[k]-r;
					r = 0;
					if(s<0)
					{
						s += 65536;
						r = 1;
					}
					svm_value[k] = s & 0xFFFF;
				}
				for(var k=0 ; k<4 ; ++k)
				{
					svm_matrix[j+7-2*k]=svm_value[k] & 0xFF;
					svm_matrix[j+7-(2*k+1)]=svm_value[k] >>> 8;
				}
			}
			svm_transpose(svm_matrix,16);
			for(var j=0 ; j<16 ; ++j)
			{
				var cc = (svm_key[j]>>>4) & 0xF;
				var svm_line = (svm_matrix[2*j] << 8) + (svm_matrix[2*j+1]);
				var svm_shifted_line = svm_circular_shift_16(16-cc,svm_line);
				svm_matrix[2*j] = svm_shifted_line >>> 8;
				svm_matrix[2*j+1] = svm_shifted_line & 0xFF;
			}
			svm_transpose(svm_matrix,16);
			for(var j=0 ; j<16 ; ++j)
			{
				var cc = svm_key[j] & 0xF;
				var svm_line = (svm_matrix[2*j] << 8) + (svm_matrix[2*j+1]);
				var svm_shifted_line = svm_circular_shift_16(16-cc,svm_line);
				svm_matrix[2*j] = svm_shifted_line >>> 8;
				svm_matrix[2*j+1] = svm_shifted_line & 0xFF;
			}
			svm_round = svm_round.concat(svm_matrix);
		}
		var svm_size = 0;
		for(var i=0 ; i<5 ; ++i)
		{
			var svm_parity=0;
			var element = svm_round[svm_round.length-5+i];
			while(element>0)
			{
				svm_parity ^= (element&1);
				element >>>=1;
			}
			svm_size <<= 1;
			svm_size +=svm_parity;
		}
		if(svm_size<=5)
		{
			svm_size += 32;
		}
		if(svm_size>svm_round.length)
		{
			return '';
		}
		mesg = svm_round.slice(0,svm_round.length-svm_size);
	}
	return mesg.map(function(e) { return String.fromCharCode(e); }).join('');;
}
