/*
 ---------------------------------------------------------------------------
 Copyright (c) 2016, ������о�Ƽ����޹�˾, All rights reserved.
 ˵���� AES�����㷨��ECBģʽ��PKCS#7
 ���ߣ� ethan@senthink.com
 ʱ�䣺 2016/04/25
 ---------------------------------------------------------------------------
 ���ܹ��̣�
 1. ��ʼ������key��AES�����ģ�aes_set_key
 2. ���ݴ��������ݳ��ȣ���������ܺ����ݵĳ��ȣ�compute_out_length
 3. �Դ��������ݽ���pkcs#7��䣻 pkcs7_padding
 4. ����������ݽ��м��ܣ� aes_ecb_encrypt
 ���ܹ��̣�
 1. �Դ��������ݽ��н���(���ܺ�����к���padding)��aes_ecb_decrypt
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
 * ��ʼ��Key��AES Context
 * param�� key ��Կ��
 *         keylen ��Կ����(�Ϸ����ݣ�128, 192, 16, 24 �� 32)��
 *         ctx ������
 * return: 0�ɹ���-1ʧ��
 */
uint8_t aes_set_key( const unsigned char key[], uint8_t keylen, aes_context ctx[1] );

/**
 * ����16�ֽڵĿ����
 * param�� in ���������ݣ�
 *         out ���ܺ������
 *         ctx ������
 * return: 0�ɹ���-1ʧ��
 */
uint8_t aes_encrypt( const unsigned char in[N_BLOCK], unsigned char out[N_BLOCK], const aes_context ctx[1] );

/**
 * �������������ݳ���
 * param�� in_len ���������ݳ��ȣ�
 * return: 0�ɹ���-1ʧ��
 */
int compute_out_length(int in_len);

/**
 * pkcs#7���
 * param�� in ��������ݣ�
 *         out �������ݣ�
 *         in_len ��������ݵĳ���;
 *         out_len �������ݵĳ��ȣ��� compute_out_length�����)
 * return: void
 */
void pkcs7_padding(const unsigned char *in, unsigned char *out, int in_len, int out_len);

/**
 * aes ecb ����
 * param�� in ����������(���������)��
 *         out ���ܺ�������ݣ�
 *         len ���ܺ�������ݳ���;
 *         ctx AES������
 * return: 0 �ɹ���-1 ʧ��
 */
uint8_t aes_ecb_encrypt(const unsigned char *in, unsigned char *out, int len, const aes_context ctx[1]);

/**
 * aes ecb ���ܵ���16�ֽڵĿ�
 * param�� in ���������ݣ�
 *         out ���ܺ�������ݣ�
 *         ctx AES������
 * return: 0 �ɹ���-1 ʧ��
 */
uint8_t aes_decrypt( const unsigned char in[N_BLOCK], unsigned char out[N_BLOCK], const aes_context ctx[1] );

/**
 * aes ecb ����
 * param�� in ���������ݣ�
 *         out ���ܺ��������(������ݺ�padding����Ҫ�Լ�ȥ��)��
 *         in_len ���������ݳ���
 *         ctx AES������
 * return: 0 �ɹ���-1 ʧ��
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
