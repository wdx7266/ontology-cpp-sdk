#include "AES.h"

bool AES::set_key(const std::string &k) {
  if (k.length() != AES_256_KEY_SIZE) {
    return false;
  }
  std::copy(k.begin(), k.end(), params->key);
  return true;
}
bool AES::set_iv(const std::string &v) {
  if (v.length() != AES_BLOCK_SIZE) {
    return false;
  }
  std::copy(v.begin(), v.end(), params->iv);
  return true;
}

bool AES::set_mode(AEAD_mode mode) {
  /* Set the cipher type for encryption-decryption */
  switch (mode) {
  case AES_CTR:
    params->cipher_type = EVP_aes_256_ctr();
    break;
  case AES_GCM:
    params->cipher_type = EVP_aes_256_gcm();
    break;
  default:
    return false;
  }
  return true;
}

bool AES::set_params(const std::string &k, const std::string &v,
                     const AEAD_mode mode) {
  if (!set_key(k)) {
    return false;
  }
  if (!set_iv(v)) {
    return false;
  }
  if (!set_mode(mode)) {
    return false;
  }
  return true;
}

bool AES::params_init(AEAD_mode mode) {
  /* Generate cryptographically strong pseudo-random bytes for key and IV */
  // ENGINE *engine;

  // ENGINE_load_rdrand();

  // engine = ENGINE_by_id("rdrand");
  // if (engine == NULL) {
  //   return false;
  // }

  if (RAND_bytes(params->key, AES_256_KEY_SIZE) != 1) {
    return false;
  }
  if (RAND_bytes(params->iv, AES_BLOCK_SIZE) != 1) {
    return false;
  }
  if (set_mode(mode) == false) {
    return false;
  }
  return true;
}

bool AES::auth_encry(std::string msg, std::string &enc_msg) {
  EVP_CIPHER_CTX *ctx;
  ctx = EVP_CIPHER_CTX_new();
  EVP_CIPHER_CTX_init(ctx);
  if (ctx == NULL) {
    return false;
  }

  EVP_EncryptInit_ex(ctx, params->cipher_type, NULL, params->key, params->iv);

  /* check lengths */
  if (EVP_CIPHER_CTX_key_length(ctx) != AES_256_KEY_SIZE) {
    cout << EVP_CIPHER_CTX_key_length(ctx) << endl;
    return false;
  }
  if (EVP_CIPHER_CTX_iv_length(ctx) != AES_BLOCK_SIZE) {
    cout << EVP_CIPHER_CTX_iv_length(ctx) << endl;
    return false;
  }

  /* set key and IV */
  if (EVP_EncryptInit_ex(ctx, params->cipher_type, NULL, params->key,
                         params->iv) != 1) {
    return false;
  }

  /* Allow enough space in output buffer for additional block */
  int cipher_block_size = EVP_CIPHER_block_size(params->cipher_type);
  int buf_size = (int)msg.length();
  unsigned char out_buf[buf_size + cipher_block_size];

  int out_len;
  if (EVP_EncryptUpdate(ctx, out_buf, &out_len,
                        (const unsigned char *)msg.c_str(),
                        msg.length()) != 1) {
    EVP_CIPHER_CTX_cleanup(ctx);
    return false;
  }

  if (EVP_EncryptFinal_ex(ctx, out_buf, &out_len) != 1) {
    EVP_CIPHER_CTX_cleanup(ctx);
    return false;
  }

  enc_msg = std::string(reinterpret_cast<char *>(out_buf));
  EVP_CIPHER_CTX_cleanup(ctx);
  return true;
}
bool AES::auth_decry(std::string msg, std::string &dec_msg) {
  int is_encrypt = 0;
  /* Allow enough space in output buffer for additional block */
  int cipher_block_size = EVP_CIPHER_block_size(params->cipher_type);

  EVP_CIPHER_CTX *ctx;
  ctx = EVP_CIPHER_CTX_new();
  EVP_CIPHER_CTX_init(ctx);
  if (ctx == NULL) {
    return false;
  }

  /* set key and IV */
  if (EVP_DecryptInit_ex(ctx, params->cipher_type, NULL, params->key,
                         params->iv) != 1) {
    return false;
  }

  int buf_size = (int)msg.length();
  unsigned char out_buf[buf_size + cipher_block_size];

  int out_len;
  if (EVP_DecryptUpdate(ctx, out_buf, &out_len,
                        (const unsigned char *)msg.c_str(),
                        msg.length()) != 1) {
    EVP_CIPHER_CTX_cleanup(ctx);
    return false;
  }

  if (EVP_DecryptFinal_ex(ctx, out_buf, &out_len) != 1) {
    EVP_CIPHER_CTX_cleanup(ctx);
    return false;
  }

  dec_msg = std::string(reinterpret_cast<char *>(out_buf));
  EVP_CIPHER_CTX_cleanup(ctx);
  return true;
}