#ifndef SIGN_H
#define SIGN_H

#include <openssl/bio.h>
#include <openssl/conf.h>
#include <openssl/ec.h>    // for EC_GROUP_new_by_curve_name, EC_GROUP_free, EC_KEY_new, EC_KEY_set_group, EC_KEY_generate_key, EC_KEY_free
#include <openssl/ecdsa.h> // for ECDSA_do_sign, ECDSA_do_verify
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/obj_mac.h> // for NID_secp192k1
#include <openssl/pem.h>

#include <cstring>
#include <iostream>
#include <stdio.h>
#include <string>

using namespace std;

enum SignatureScheme {
  SHA224withECDSA,
  SHA256withECDSA,
  SHA384withECDSA,
  SHA512withECDSA,
  SHA3_224withECDSA,
  SHA3_256withECDSA,
  SHA3_384withECDSA,
  SHA3_512withECDSA,
  RIPEMD160withECDSA,
  SM3withSM2
};

enum CurveName {
  p224 = NID_secp224k1,
  /* ANSI X9.62 Prime 256v1 curve */
  p256 = NID_X9_62_prime256v1,
  p384 = NID_secp384r1,
  p521 = NID_secp521r1,
  SM2P256V1 = NID_sm2
};

class Sign {
private:
  EVP_PKEY *key;
  EC_KEY *ec_key;
  bool md_ctx_sign_init(const SignatureScheme sign_scheme, EVP_MD_CTX *md_ctx);
  bool md_ctx_veri_init(const SignatureScheme sign_scheme, EVP_MD_CTX *md_ctx);
  bool md_ctx_digest_init(const SignatureScheme sign_scheme,
                          EVP_MD_CTX *md_ctx);

protected:
  bool EC_set_public_key(const string &str_public_key, CurveName curve_nid);
  bool EC_set_private_key(const string &str_private_key, CurveName curve_nid);

public:
  Sign() { key = EVP_PKEY_new(); }
  ~Sign() {
    if (ec_key != NULL) {
      EC_KEY_free(ec_key);
    }
    EVP_cleanup();
  }

  bool EC_init(CurveName curve_nid = p256);
  bool ECDSA_key_generate(CurveName curve_nid = p256);
  bool EC_get_public_key(string &str_public_key);
  bool EC_get_private_key(string &str_private_key);
  bool EC_set_key(const string &str_public_key, const string &str_private_key,
                  CurveName curve_nid);
  bool EC_get_pubkey_by_prikey(const string &str_private_key,
                               string &str_public_key, CurveName curve_nid);
  bool EC_sign(const std::string &msg, std::string &str_sign_dgst,
               SignatureScheme sign_scheme = SHA256withECDSA);
  bool EC_veri(const std::string &msg, std::string &str_sign_dgst,
               SignatureScheme sign_scheme = SHA256withECDSA);

  // bool SM_sign(const std::string &msg, std::string str_sign_dgst,
  //              SignatureScheme sign_scheme = SM3withSM2);
};

#endif