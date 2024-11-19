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

#pragma once
#include <src/global/global.h>

namespace SVM
{
namespace Machine
{
namespace Debugueur
{
struct Securite
{
	static std::string encode_base64(const std::string& source)
	{
		static const unsigned char base64_caracteres[65] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
		std::ostringstream oss;
		const unsigned char *fin_source, *it;

		it = reinterpret_cast<const unsigned char*>(source.c_str());
		fin_source = it + source.size();
		while (fin_source - it >= 3) {
			oss << base64_caracteres[it[0] >> 2];
			oss << base64_caracteres[((it[0] & 0x03) << 4) | (it[1] >> 4)];
			oss << base64_caracteres[((it[1] & 0x0f) << 2) | (it[2] >> 6)];
			oss << base64_caracteres[it[2] & 0x3f];
			it += 3;
		}

		if (fin_source - it) {
			oss << base64_caracteres[it[0] >> 2];
			if (fin_source - it == 1) {
				oss << base64_caracteres[(it[0] & 0x03) << 4];
				oss << '=';
			}
			else {
				oss << base64_caracteres[((it[0] & 0x03) << 4) | (it[1] >> 4)];
				oss << base64_caracteres[(it[1] & 0x0f) << 2];
			}
			oss << '=';
		}

		return oss.str();
	}

	static std::string decode_base64(const std::string& source)
	{
		static const int base64_index[256] = { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 62, 63, 62, 62, 63, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61,  0,  0,  0,  0,  0,  0,  0,  0,  1,  2,  3,  4,  5,  6, 7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,  0, 0,  0,  0, 63,  0, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51 };
		const unsigned char* p = reinterpret_cast<const unsigned char*>(source.c_str()); 
		int extra = source.size() > 0 && (source.size() % 4 || p[source.size() - 1] == '=');
		const size_t longueur = ((source.size() + 3) / 4 - extra) * 4;
		std::string resultat(longueur / 4 * 3 + extra, '\0');

		for (size_t i = 0, j = 0; i < longueur; i += 4)
		{
			int n = base64_index[p[i]] << 18 | base64_index[p[i + 1]] << 12 | base64_index[p[i + 2]] << 6 | base64_index[p[i + 3]];
			resultat[j++] = n >> 16;
			resultat[j++] = n >> 8 & 0xFF;
			resultat[j++] = n & 0xFF;
		}
		if (extra)
		{
			int n = base64_index[p[longueur]] << 18 | base64_index[p[longueur + 1]] << 12;
			resultat[resultat.size() - 1] = n >> 16;

			if (source.size() > longueur + 2 && p[longueur + 2] != '=')
			{
				n |= base64_index[p[longueur + 2]] << 6;
				resultat.push_back(n >> 8 & 0xFF);
			}
		}
		return resultat;
	}

	static std::string encode_hex(const std::string& source)
	{
		std::string resultat;
		for(const auto& c: source)
		{
			unsigned char cc=c;
			unsigned char hc = cc >> 4;
			unsigned char lc = cc & 0xF;
			resultat += Securite::caractere_hex(hc);
			resultat += Securite::caractere_hex(lc);
		}
		return resultat;
	}
	static std::string decode_hex(const std::string& source)
	{
		std::string resultat;
		bool pair = true;
		unsigned char r;
		for(const auto& c: source)
		{
			unsigned char cc=Securite::entier_hex(c);
			if(pair)
			{
				r = cc << 4;
			}
			else
			{
				r += cc;
				resultat += r;
			}
			pair = not pair;
		}
		return resultat;
	}

	static std::string hashe_sha1(const std::string& source)
	{
		Atome hash[5];
		hash[0] = 0x67452301;
		hash[1] = 0xEFCDAB89;
		hash[2] = 0x98BADCFE;
		hash[3] = 0x10325476;
		hash[4] = 0xC3D2E1F0;
		unsigned char bloc[64];
		bool debut_extra = true;
		for(size_t index_bloc = 0 ; ; index_bloc += 64)
		{
			bool fin = false;
			for(size_t index=0 ; index<64 ; ++index)
			{
				if(index_bloc+index<source.size())
				{
					bloc[index] = source[index_bloc+index];
				}
				else
				{
					if(debut_extra)
					{
						bloc[index] = 0x80;
						debut_extra = false;
						if(index<=55) fin = true;
					}
					else
					{
						bloc[index] = 0;
						if(index==0) fin = true;
					}
					if(fin and (index>55))
					{
						bloc[index] = ((8*source.size()) >> (8*(63-index))) bitand 0xFF;
					}
				}
			}
			//std::cout << "Bloc";
			//for(size_t i=0 ; i<64 ; ++i)
			//{
			//	std::cout << " " << Securite::encode_hex(std::string(1,bloc[i]));
			//}
			//std::cout << std::endl;
			static const Atome K[] = { 0x5A827999, 0x6ED9EBA1, 0x8F1BBCDC, 0xCA62C1D6 };
			size_t i;
			Atome temporaire;
			Atome W[80];
			Atome A = hash[0];
			Atome B = hash[1];
			Atome C = hash[2];
			Atome D = hash[3];
			Atome E = hash[4];
			for(i=0 ; i<16 ; ++i)
			{
				W[i] = (bloc[i*4] << 24) bitor (bloc[4*i+1] << 16) bitor (bloc[4*i+2] << 8) bitor (bloc[4*i+3]);
			}
			for(i=16 ; i<80 ; ++i)
			{
				W[i] = decalage_circulaire(1,W[i-3] ^ W[i-8] ^ W[i-14] ^ W[i-16]);
			}
			for(i=0 ; i<20 ; ++i)
			{
				temporaire =  decalage_circulaire(5,A) + ((B & C) | ((~B) & D)) + E + W[i] + K[0];
				E = D;
				D = C;
				C = decalage_circulaire(30,B);
				B = A;
				A = temporaire;
			}
			for(i=20 ; i<40 ; ++i)
			{
				temporaire = decalage_circulaire(5,A) + (B ^ C ^ D) + E + W[i] + K[1];
				E = D;
				D = C;
				C = decalage_circulaire(30,B);
				B = A;
				A = temporaire;
			}
			for(i=40 ; i<60 ; ++i)
			{
				temporaire = decalage_circulaire(5,A) + ((B & C) | (B & D) | (C & D)) + E + W[i] + K[2];
				E = D;
				D = C;
				C = decalage_circulaire(30,B);
				B = A;
				A = temporaire;
			}
			for(i=60 ; i<80 ; ++i)
			{
				temporaire = decalage_circulaire(5,A) + (B ^ C ^ D) + E + W[i] + K[3];
				E = D;
				D = C;
				C = decalage_circulaire(30,B);
				B = A;
				A = temporaire;
			}
			hash[0] += A;
			hash[1] += B;
			hash[2] += C;
			hash[3] += D;
			hash[4] += E;
			if(fin) break;
		}
		unsigned char resultat[20]="";
		for(size_t i=0 ; i<20 ; ++i)
		{
			resultat[i] = hash[i>>2] >> 8*(3-(i bitand 0x03));
		}
		return std::string(reinterpret_cast<char*>(resultat),20);
	}
	
	static std::string genere_aleatoire(const size_t taille)
	{
		static std::random_device rd;
		static std::mt19937 generateur(rd());
		std::string resultat(taille,'\0');
		std::uniform_int_distribution<> distribution(0,255);
		for(size_t i=0 ; i<taille ; ++i)
		{
			resultat[i] = distribution(generateur);
		}
		return resultat;
	}

	static void reinitialise(std::string& chaine)
	{
		for(auto& c: chaine)
		{
			c='\0';
		}
	}

	private:	
	typedef uint32_t Atome;
	static unsigned char caractere_hex(unsigned char c)
	{
		if(c>=10)
			return 'a'+c-10;
		return '0'+c;
	}

	static unsigned char entier_hex(unsigned char c)
	{
		if(c>='a' and c<='f')
			return 10+c-'a';
		return c-'0';
	}
	template<typename T>
	static T decalage_circulaire(const char decalage, const T entree)
	{
		return (entree << decalage) bitor (entree >> (8*sizeof(T)-decalage));
	}
};

}
}
}
