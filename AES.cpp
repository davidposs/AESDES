#include "AES.h"

/**
 * Sets the key to use
 * @param key - the first byte of this represents whether
 * to encrypt or to decrypt. 00 means encrypt and any other
 * value to decrypt.  Then come the bytes of the 128-bit key
 * (should be 16 of them).
 * @return - True if the key is valid and False otherwise
 */
bool AES::setKey(const unsigned char* keyArray)
{
	
  // TODO: AES implementation of openssl cares about whether
	// you are encrypting or decrypting when setting the key.
	// That is, when encrypting you use function AES_set_encrypt_key(...)
	// and when decrypting AES_set_decrypt_key(...).
	// One way to solve this problem is to pass in a 17 byte key, where
	// the first byte is used to indicate whether we are encrypting or
	// decrypting. E.g., if the first byte is 0, then use AES_set_encrypt_key(...).

  //Encrypting
  
  if (keyArray[0] == 0x00) {
    if (AES_set_encrypt_key(keyArray + 1, 128, &(this->enc_key)) !=0) {
      printf("Key error, exiting\n");
      exit(-1);
    }
    return true;
  }

  //Decrypting
  
  else if (keyArray[0] == 0x01) {
    AES_KEY dec_key;
    if (AES_set_decrypt_key(keyArray + 1, 128, &(this->dec_key)) != 0) {
      printf("Key error, exiting\n");
      exit(-1);
    }
    return true;
  }
  	
  else {
    return false;
  }	
}

/**	
 * Encrypts a plaintext string
 * @param plaintext - the plaintext string
 * @return - the encrypted ciphertext string
 */
unsigned char* AES::encrypt(const unsigned char* plaintext)
{
  unsigned char* ciphertext = new unsigned char[16];
  AES_ecb_encrypt(plaintext, ciphertext, &(this->enc_key), AES_ENCRYPT);
  return ciphertext;
}

/**
 * Decrypts a string of ciphertext
 * @param cipherText - the ciphertext
 * @return - the plaintext
 */
unsigned char* AES::decrypt(const unsigned char* ciphertext)
{
  unsigned char* plaintext = new unsigned char[16];
  AES_ecb_encrypt(ciphertext, plaintext, &(this->dec_key), AES_DECRYPT);
  return plaintext;		
}
