// MD5.java MD5 message-digest algorithm

// convert from MD5C.C - RSA Data Security, Inc.,
// Taisuke Fukuno (uni@po.miyako.co.jp) http://sariel.miyako.co.jp/~uni/

/* Copyright (C) 1991-2, RSA Data Security, Inc. Created 1991. All
rights reserved.

License to copy and use this software is granted provided that it
is identified as the "RSA Data Security, Inc. MD5 Message-Digest
Algorithm" in all material mentioning or referencing this software
or this function.

License is also granted to make and use derivative works provided
that such works are identified as "derived from the RSA Data
Security, Inc. MD5 Message-Digest Algorithm" in all material
mentioning or referencing the derived work.

RSA Data Security, Inc. makes no representations concerning either
the merchantability of this software or the suitability of this
software for any particular purpose. It is provided "as is"
without express or implied warranty of any kind.

These notices must be retained in any copies of any part of this
documentation and/or software.
 */

public class MD5 {
	public static void main(String [] args) {
	    System.out.println(new MD5().toDigest(args[0]));
	}
	
	static final char[] HEX = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };
	public String toDigest(String string) {
		byte[] digest = toDigest(string.getBytes());
		StringBuffer sb = new StringBuffer();
		for (int i = 0; i < digest.length; i++) {
			sb.append(HEX[byte2int(digest[i]) / 16]);
			sb.append(HEX[byte2int(digest[i]) % 16]);
		}
		return sb.toString();
	}
	public byte[] toDigest(byte[] string) {
		MD5_CTX context = new MD5_CTX();
		byte[] digest = new byte[16];
		int len = string.length;
		MD5Init(context);
		MD5Update(context, string, len);
		MD5Final(digest, context);
		return digest;
	}
	
	static final int S11 = 7;
	static final int S12 = 12;
	static final int S13 = 17;
	static final int S14 = 22;
	static final int S21 = 5;
	static final int S22 = 9;
	static final int S23 = 14;
	static final int S24 = 20;
	static final int S31 = 4;
	static final int S32 = 11;
	static final int S33 = 16;
	static final int S34 = 23;
	static final int S41 = 6;
	static final int S42 = 10;
	static final int S43 = 15;
	static final int S44 = 21;

	static final byte[] PADDING = new byte[64];
	static {
		PADDING[0] = (byte)0x80;
		for (int i = 1; i < PADDING.length; i++)
			PADDING[i] = (byte)0;
	}
	
	long fix(long l) {
		return l & 0xffffffffl;
	}

	/* F, G, H and I are basic MD5 functions.
	 */
	long F(long x, long y, long z) {
		return fix((x & y) | (~x & z));
	}
	long G(long x, long y, long z) {
		return fix((x & z) | (y & ~z));
	}
	long H(long x, long y, long z) {
		return fix(x ^ y ^ z);
	}
	long I(long x, long y, long z) {
		return fix(y ^ (x | ~z));
	}

	/* ROTATE_LEFT rotates x left n bits.
	 */
	long ROTATE_LEFT(long x, long n) {
		return fix((x << n) | (x >> (32 - n)));
	}

	/* FF, GG, HH, and II transformations for rounds 1, 2, 3, and 4.
	Rotation is separate from addition to prevent recomputation.
	 */
	void FF(long[] n, int a, int b, int c, int d, long x, long s, long ac) {
		n[a] += F(n[b], n[c], n[d]) + x + ac;
		n[a] = ROTATE_LEFT(fix(n[a]), s);
		n[a] += n[b];
		n[a] = fix(n[a]);
	  }
	void GG(long[] n, int a, int b, int c, int d, long x, long s, long ac) {
		n[a] += G(n[b], n[c], n[d]) + x + ac;
		n[a] = ROTATE_LEFT(fix(n[a]), s);
		n[a] += n[b];
		n[a] = fix(n[a]);
	  }
	void HH(long[] n, int a, int b, int c, int d, long x, long s, long ac) {
		n[a] += H(n[b], n[c], n[d]) + x + ac;
		n[a] = ROTATE_LEFT(fix(n[a]), s);
		n[a] += n[b];
		n[a] = fix(n[a]);
	  }
	void II(long[] n, int a, int b, int c, int d, long x, long s, long ac) {
		n[a] += I(n[b], n[c], n[d]) + x + ac;
		n[a] = ROTATE_LEFT(fix(n[a]), s);
		n[a] += n[b];
		n[a] = fix(n[a]);
	}
	
	class MD5_CTX {
	  long[] state = new long[4];                                   /* state (ABCD) */
	  long[] count = new long[2];        /* number of bits, modulo 2^64 (lsb first) */
	  byte[] buffer = new byte[64];                         /* input buffer */
	}
	/* MD5 initialization. Begins an MD5 operation, writing a new context.
	 */
	void MD5Init(MD5_CTX context) {
	  context.count[0] = context.count[1] = 0;
	  /* Load magic initialization constants.
	*/
	  context.state[0] = 0x67452301l;
	  context.state[1] = 0xefcdab89l;
	  context.state[2] = 0x98badcfel;
	  context.state[3] = 0x10325476l;
	}

	/* MD5 block update operation. Continues an MD5 message-digest
	  operation, processing another message block, and updating the
	  context.
	 */
	void MD5Update(MD5_CTX context, byte[] input, int inputLen) {
	  int i, index, partLen;

	  /* Compute number of bytes mod 64 */
	  index = (int)((context.count[0] >> 3) & 0x3F);

	  /* Update number of bits */
	  if (fix(context.count[0] += ((long)inputLen << 3)) < fix((long)inputLen << 3))
		context.count[1]++;
	  context.count[1] += ((long)inputLen >> 29);
	  context.count[1] = fix(context.count[1]);

	  partLen = 64 - index;

	  /* Transform as many times as possible.
	*/
	  if (inputLen >= partLen) {
		 MD5_memcpy(context.buffer, index, input, 0, partLen);
		 MD5Transform(context.state, context.buffer, 0);

		for (i = partLen; i + 63 < inputLen; i += 64)
			MD5Transform(context.state, input, i);

		 index = 0;
	  }
	  else
		i = 0;

	  /* Buffer remaining input */
	  MD5_memcpy(context.buffer, index, input, i, inputLen-i);
	}

	/* MD5 finalization. Ends an MD5 message-digest operation, writing the
	  the message digest and zeroizing the context.
	 */
	void MD5Final(byte[] digest, MD5_CTX context) {
	  byte[] bits = new byte[8];
	  int index, padLen;

	  /* Save number of bits */
	  Encode(bits, context.count, 8);

	  /* Pad out to 56 mod 64.
	*/
	  index = (int)((context.count[0] >> 3) & 0x3f);
	  padLen = (index < 56) ? (56 - index) : (120 - index);
	  MD5Update(context, PADDING, padLen);

	  /* Append length (before padding) */
	  MD5Update(context, bits, 8);

	  /* Store state in digest */
	  Encode(digest, context.state, 16);
	}

	/* MD5 basic transformation. Transforms state based on block.
	 */
	void MD5Transform(long[] state, byte[] block, int idx) {
	  long[] buf = new long[4];
	  System.arraycopy(state, 0, buf, 0, 4);
	  long[] x = new long[16];
	  Decode(x, block, idx, 64);
	  
	  /* Round 1 */
	  FF(buf, 0, 1, 2, 3, x[ 0], S11, 0xd76aa478l); /* 1 */
	  FF(buf, 3, 0, 1, 2, x[ 1], S12, 0xe8c7b756l); /* 2 */
	  FF(buf, 2, 3, 0, 1, x[ 2], S13, 0x242070dbl); /* 3 */
	  FF(buf, 1, 2, 3, 0, x[ 3], S14, 0xc1bdceeel); /* 4 */
	  FF(buf, 0, 1, 2, 3, x[ 4], S11, 0xf57c0fafl); /* 5 */
	  FF(buf, 3, 0, 1, 2, x[ 5], S12, 0x4787c62al); /* 6 */
	  FF(buf, 2, 3, 0, 1, x[ 6], S13, 0xa8304613l); /* 7 */
	  FF(buf, 1, 2, 3, 0, x[ 7], S14, 0xfd469501l); /* 8 */
	  FF(buf, 0, 1, 2, 3, x[ 8], S11, 0x698098d8l); /* 9 */
	  FF(buf, 3, 0, 1, 2, x[ 9], S12, 0x8b44f7afl); /* 10 */
	  FF(buf, 2, 3, 0, 1, x[10], S13, 0xffff5bb1l); /* 11 */
	  FF(buf, 1, 2, 3, 0, x[11], S14, 0x895cd7bel); /* 12 */
	  FF(buf, 0, 1, 2, 3, x[12], S11, 0x6b901122l); /* 13 */
	  FF(buf, 3, 0, 1, 2, x[13], S12, 0xfd987193l); /* 14 */
	  FF(buf, 2, 3, 0, 1, x[14], S13, 0xa679438el); /* 15 */
	  FF(buf, 1, 2, 3, 0, x[15], S14, 0x49b40821l); /* 16 */

	 /* Round 2 */
	  GG(buf, 0, 1, 2, 3, x[ 1], S21, 0xf61e2562l); /* 17 */
	  GG(buf, 3, 0, 1, 2, x[ 6], S22, 0xc040b340l); /* 18 */
	  GG(buf, 2, 3, 0, 1, x[11], S23, 0x265e5a51l); /* 19 */
	  GG(buf, 1, 2, 3, 0, x[ 0], S24, 0xe9b6c7aal); /* 20 */
	  GG(buf, 0, 1, 2, 3, x[ 5], S21, 0xd62f105dl); /* 21 */
	  GG(buf, 3, 0, 1, 2, x[10], S22,  0x2441453l); /* 22 */
	  GG(buf, 2, 3, 0, 1, x[15], S23, 0xd8a1e681l); /* 23 */
	  GG(buf, 1, 2, 3, 0, x[ 4], S24, 0xe7d3fbc8l); /* 24 */
	  GG(buf, 0, 1, 2, 3, x[ 9], S21, 0x21e1cde6l); /* 25 */
	  GG(buf, 3, 0, 1, 2, x[14], S22, 0xc33707d6l); /* 26 */
	  GG(buf, 2, 3, 0, 1, x[ 3], S23, 0xf4d50d87l); /* 27 */
	  GG(buf, 1, 2, 3, 0, x[ 8], S24, 0x455a14edl); /* 28 */
	  GG(buf, 0, 1, 2, 3, x[13], S21, 0xa9e3e905l); /* 29 */
	  GG(buf, 3, 0, 1, 2, x[ 2], S22, 0xfcefa3f8l); /* 30 */
	  GG(buf, 2, 3, 0, 1, x[ 7], S23, 0x676f02d9l); /* 31 */
	  GG(buf, 1, 2, 3, 0, x[12], S24, 0x8d2a4c8al); /* 32 */

	  /* Round 3 */
	  HH(buf, 0, 1, 2, 3, x[ 5], S31, 0xfffa3942l); /* 33 */
	  HH(buf, 3, 0, 1, 2, x[ 8], S32, 0x8771f681l); /* 34 */
	  HH(buf, 2, 3, 0, 1, x[11], S33, 0x6d9d6122l); /* 35 */
	  HH(buf, 1, 2, 3, 0, x[14], S34, 0xfde5380cl); /* 36 */
	  HH(buf, 0, 1, 2, 3, x[ 1], S31, 0xa4beea44l); /* 37 */
	  HH(buf, 3, 0, 1, 2, x[ 4], S32, 0x4bdecfa9l); /* 38 */
	  HH(buf, 2, 3, 0, 1, x[ 7], S33, 0xf6bb4b60l); /* 39 */
	  HH(buf, 1, 2, 3, 0, x[10], S34, 0xbebfbc70l); /* 40 */
	  HH(buf, 0, 1, 2, 3, x[13], S31, 0x289b7ec6l); /* 41 */
	  HH(buf, 3, 0, 1, 2, x[ 0], S32, 0xeaa127fal); /* 42 */
	  HH(buf, 2, 3, 0, 1, x[ 3], S33, 0xd4ef3085l); /* 43 */
	  HH(buf, 1, 2, 3, 0, x[ 6], S34,  0x4881d05l); /* 44 */
	  HH(buf, 0, 1, 2, 3, x[ 9], S31, 0xd9d4d039l); /* 45 */
	  HH(buf, 3, 0, 1, 2, x[12], S32, 0xe6db99e5l); /* 46 */
	  HH(buf, 2, 3, 0, 1, x[15], S33, 0x1fa27cf8l); /* 47 */
	  HH(buf, 1, 2, 3, 0, x[ 2], S34, 0xc4ac5665l); /* 48 */

	  /* Round 4 */
	  II(buf, 0, 1, 2, 3, x[ 0], S41, 0xf4292244l); /* 49 */
	  II(buf, 3, 0, 1, 2, x[ 7], S42, 0x432aff97l); /* 50 */
	  II(buf, 2, 3, 0, 1, x[14], S43, 0xab9423a7l); /* 51 */
	  II(buf, 1, 2, 3, 0, x[ 5], S44, 0xfc93a039l); /* 52 */
	  II(buf, 0, 1, 2, 3, x[12], S41, 0x655b59c3l); /* 53 */
	  II(buf, 3, 0, 1, 2, x[ 3], S42, 0x8f0ccc92l); /* 54 */
	  II(buf, 2, 3, 0, 1, x[10], S43, 0xffeff47dl); /* 55 */
	  II(buf, 1, 2, 3, 0, x[ 1], S44, 0x85845dd1l); /* 56 */
	  II(buf, 0, 1, 2, 3, x[ 8], S41, 0x6fa87e4fl); /* 57 */
	  II(buf, 3, 0, 1, 2, x[15], S42, 0xfe2ce6e0l); /* 58 */
	  II(buf, 2, 3, 0, 1, x[ 6], S43, 0xa3014314l); /* 59 */
	  II(buf, 1, 2, 3, 0, x[13], S44, 0x4e0811a1l); /* 60 */
	  II(buf, 0, 1, 2, 3, x[ 4], S41, 0xf7537e82l); /* 61 */
	  II(buf, 3, 0, 1, 2, x[11], S42, 0xbd3af235l); /* 62 */
	  II(buf, 2, 3, 0, 1, x[ 2], S43, 0x2ad7d2bbl); /* 63 */
	  II(buf, 1, 2, 3, 0, x[ 9], S44, 0xeb86d391l); /* 64 */
		
		for (int i = 0; i < 4; i++)
			state[i] = fix(state[i] + buf[i]);
	}

	/* Encodes input (UINT4) into output (unsigned char). Assumes len is
	  a multiple of 4.
	 */
	void Encode(byte[] output, long[] input, int len) {
	  int i, j;
	  for (i = 0, j = 0; j < len; i++, j += 4) {
		 output[j] = (byte)(input[i] & 0xff);
		 output[j+1] = (byte)((input[i] >> 8) & 0xff);
		 output[j+2] = (byte)((input[i] >> 16) & 0xff);
		 output[j+3] = (byte)((input[i] >> 24) & 0xff);
	  }
	}

	/* Decodes input (unsigned char) into output (UINT4). Assumes len is
	  a multiple of 4.
	 */
	void Decode(long[] output, byte[] input, int iidx, int len) {
	  int i, j;
	  for (i = 0, j = 0; j < len; i++, j += 4)
		 output[i] = ((long)byte2int(input[iidx + j]))
		          | (((long)byte2int(input[iidx + j+1])) << 8)
		          | (((long)byte2int(input[iidx + j+2])) << 16)
		          | (((long)byte2int(input[iidx + j+3])) << 24);
	}

	/* Note: Replace "for loop" with standard memcpy if possible.
	 */

	void MD5_memcpy(byte[] output, int oidx, byte[] input, int iidx, int len) {
	  int i;
	  for (i = 0; i < len; i++)
		 output[oidx + i] = input[iidx + i];
	}
	
	int byte2int(byte b) {
		if (b < 0)
			return (int)b + 0x100;
		return b;
	}
}
