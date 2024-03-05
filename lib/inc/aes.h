/*
 ---------------------------------------------------------------------------
 Copyright (c) 2016, 杭州贤芯科技有限公司, All rights reserved.
 说明： AES加密算法，ECB模式，PKCS#7
 作者： ethan@senthink.com
 时间： 2016/04/25
 ---------------------------------------------------------------------------
 加密过程：
 1. 初始化加密key和AES上下文；aes_set_key
 2. 根据待加密内容长度，计算出加密后内容的长度；compute_out_length
 3. 对待加密内容进行pkcs#7填充； pkcs7_padding
 4. 对填充后的内容进行加密； aes_ecb_encrypt
 解密过程：
 1. 对待解密内容进行解密(解密后输出中含有padding)；aes_ecb_decrypt
 */

#ifndef AES_H
#define AES_H

#ifdef __cplusplus
  extern "C" {
#endif
#include "type.h"

#define EXIT_SUCCESS 			0
#define EXIT_FAILURE 			1

#define N_ROW                   4
#define N_COL                   4
#define N_BLOCK   (N_ROW * N_COL)
#define N_MAX_ROUNDS           14
#define AES_SESSION_KEY_LENGTH     16
typedef struct
{   uint8_t ksch[(N_MAX_ROUNDS + 1) * N_BLOCK];
    uint8_t rnd;
} aes_context;

/**
 * 初始化Key和AES Context
 * param： key 密钥；
 *         keylen 密钥长度(合法数据：128, 192, 16, 24 和 32)；
 *         ctx 上下文
 * return: 0成功，-1失败
 */
uint8_t aes_set_key( const unsigned char key[], uint8_t keylen, aes_context ctx[1] );

/**
 * 单个16字节的块加密
 * param： in 待加密内容；
 *         out 加密后输出；
 *         ctx 上下文
 * return: 0成功，-1失败
 */
uint8_t aes_encrypt( const unsigned char in[N_BLOCK], unsigned char out[N_BLOCK], const aes_context ctx[1] );

/**
 * 计算加密输出内容长度
 * param： in_len 待加密内容长度；
 * return: 0成功，-1失败
 */
int compute_out_length(int in_len);

/**
 * pkcs#7填充
 * param： in 待填充内容；
 *         out 填充后内容；
 *         in_len 待填充内容的长度;
 *         out_len 填充后内容的长度（由 compute_out_length计算出)
 * return: void
 */
void pkcs7_padding(const unsigned char *in, unsigned char *out, int in_len, int out_len);

/**
 * aes ecb 加密
 * param： in 待加密内容(填充后的内容)；
 *         out 加密后输出内容；
 *         len 机密后输出内容长度;
 *         ctx AES上下文
 * return: 0 成功；-1 失败
 */
uint8_t aes_ecb_encrypt(const unsigned char *in, unsigned char *out, int len, const aes_context ctx[1]);

/**
 * aes ecb 解密单个16字节的块
 * param： in 待解密内容；
 *         out 解密后输出内容；
 *         ctx AES上下文
 * return: 0 成功；-1 失败
 */
uint8_t aes_decrypt( const unsigned char in[N_BLOCK], unsigned char out[N_BLOCK], const aes_context ctx[1] );

/**
 * aes ecb 解密
 * param： in 待解密内容；
 *         out 解密后输出内容(输出内容含padding，需要自己去掉)；
 *         in_len 待解密内容长度
 *         ctx AES上下文
 * return: 0 成功；-1 失败
 */
//uint8_t aes_ecb_decrypt(const unsigned char *in, unsigned char *out, int in_len, const aes_context ctx[1]);
int aes_ecb_decrypt( const uint8_t *in, uint8_t *out, unsigned int inlen, const aes_context ctx[1] );

//pkcs7padding and decrypt
int aes_ecb_pkcs7_encrypt( const uint8_t *in, uint8_t *out, unsigned int inlen, const aes_context ctx[1] );
//Zeropadding and decrypt
int aes_ecb_Zero_encrypt( const uint8_t *in, uint8_t *out, unsigned int inlen, const aes_context ctx[1] );
#ifdef __cplusplus
  }
#endif

#endif
