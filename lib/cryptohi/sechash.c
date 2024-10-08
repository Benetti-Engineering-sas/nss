/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#include "sechash.h"
#include "secoidt.h"
#include "secerr.h"
#include "blapi.h"
#include "pk11func.h" /* for the PK11_ calls below. */

static void *
null_hash_new_context(void)
{
    return NULL;
}

static void *
null_hash_clone_context(void *v)
{
    PORT_Assert(v == NULL);
    return NULL;
}

static void
null_hash_begin(void *v)
{
}

static void
null_hash_update(void *v, const unsigned char *input, unsigned int length)
{
}

static void
null_hash_end(void *v, unsigned char *output, unsigned int *outLen,
              unsigned int maxOut)
{
    *outLen = 0;
}

static void
null_hash_destroy_context(void *v, PRBool b)
{
    PORT_Assert(v == NULL);
}

static void *
md2_NewContext(void)
{
    return (void *)PK11_CreateDigestContext(SEC_OID_MD2);
}

static void *
md5_NewContext(void)
{
    return (void *)PK11_CreateDigestContext(SEC_OID_MD5);
}

static void *
sha1_NewContext(void)
{
    return (void *)PK11_CreateDigestContext(SEC_OID_SHA1);
}

static void *
sha224_NewContext(void)
{
    return (void *)PK11_CreateDigestContext(SEC_OID_SHA224);
}

static void *
sha256_NewContext(void)
{
    return (void *)PK11_CreateDigestContext(SEC_OID_SHA256);
}

static void *
sha384_NewContext(void)
{
    return (void *)PK11_CreateDigestContext(SEC_OID_SHA384);
}

static void *
sha512_NewContext(void)
{
    return (void *)PK11_CreateDigestContext(SEC_OID_SHA512);
}

static void *
sha3_224_NewContext(void)
{
    return (void *)PK11_CreateDigestContext(SEC_OID_SHA3_224);
}

static void *
sha3_256_NewContext(void)
{
    return (void *)PK11_CreateDigestContext(SEC_OID_SHA3_256);
}

static void *
sha3_384_NewContext(void)
{
    return (void *)PK11_CreateDigestContext(SEC_OID_SHA3_384);
}

static void *
sha3_512_NewContext(void)
{
    return (void *)PK11_CreateDigestContext(SEC_OID_SHA3_512);
}

static void *
SECHash_PK11_CloneContext(void *ctx)
{
    PK11Context *pctx = ctx;
    return PK11_CloneContext(pctx);
}

static void
SECHash_PK11_DestroyContext(void *ctx, PRBool freeit)
{
    PK11Context *pctx = ctx;
    PK11_DestroyContext(pctx, freeit);
}

void
SECHash_PK11_DigestBegin(void *ctx)
{
    PK11Context *pctx = ctx;
    SECStatus rv = PK11_DigestBegin(pctx);
    PORT_Assert(rv == SECSuccess);
    (void)rv;
}

void
SECHash_PK11_DigestOp(void *ctx, const unsigned char *in, unsigned inLen)
{
    PK11Context *pctx = ctx;
    SECStatus rv = PK11_DigestOp(pctx, in, inLen);
    PORT_Assert(rv == SECSuccess);
    (void)rv;
}

void
SECHash_PK11_DigestFinal(void *ctx, unsigned char *data,
                         unsigned int *outLen, unsigned int length)
{
    PK11Context *pctx = ctx;
    SECStatus rv = PK11_DigestFinal(pctx, data, outLen, length);
    PORT_Assert(rv == SECSuccess);
    (void)rv;
}

const SECHashObject SECHashObjects[] = {
    { 0,
      null_hash_new_context,
      null_hash_clone_context,
      null_hash_destroy_context,
      null_hash_begin,
      null_hash_update,
      null_hash_end,
      0,
      HASH_AlgNULL },
    { MD2_LENGTH,
      md2_NewContext,
      SECHash_PK11_CloneContext,
      SECHash_PK11_DestroyContext,
      SECHash_PK11_DigestBegin,
      SECHash_PK11_DigestOp,
      SECHash_PK11_DigestFinal,
      MD2_BLOCK_LENGTH,
      HASH_AlgMD2 },
    { MD5_LENGTH,
      md5_NewContext,
      SECHash_PK11_CloneContext,
      SECHash_PK11_DestroyContext,
      SECHash_PK11_DigestBegin,
      SECHash_PK11_DigestOp,
      SECHash_PK11_DigestFinal,
      MD5_BLOCK_LENGTH,
      HASH_AlgMD5 },
    { SHA1_LENGTH,
      sha1_NewContext,
      SECHash_PK11_CloneContext,
      SECHash_PK11_DestroyContext,
      SECHash_PK11_DigestBegin,
      SECHash_PK11_DigestOp,
      SECHash_PK11_DigestFinal,
      SHA1_BLOCK_LENGTH,
      HASH_AlgSHA1 },
    { SHA256_LENGTH,
      sha256_NewContext,
      SECHash_PK11_CloneContext,
      SECHash_PK11_DestroyContext,
      SECHash_PK11_DigestBegin,
      SECHash_PK11_DigestOp,
      SECHash_PK11_DigestFinal,
      SHA256_BLOCK_LENGTH,
      HASH_AlgSHA256 },
    { SHA384_LENGTH,
      sha384_NewContext,
      SECHash_PK11_CloneContext,
      SECHash_PK11_DestroyContext,
      SECHash_PK11_DigestBegin,
      SECHash_PK11_DigestOp,
      SECHash_PK11_DigestFinal,
      SHA384_BLOCK_LENGTH,
      HASH_AlgSHA384 },
    { SHA512_LENGTH,
      sha512_NewContext,
      SECHash_PK11_CloneContext,
      SECHash_PK11_DestroyContext,
      SECHash_PK11_DigestBegin,
      SECHash_PK11_DigestOp,
      SECHash_PK11_DigestFinal,
      SHA512_BLOCK_LENGTH,
      HASH_AlgSHA512 },
    { SHA224_LENGTH,
      sha224_NewContext,
      SECHash_PK11_CloneContext,
      SECHash_PK11_DestroyContext,
      SECHash_PK11_DigestBegin,
      SECHash_PK11_DigestOp,
      SECHash_PK11_DigestFinal,
      SHA224_BLOCK_LENGTH,
      HASH_AlgSHA224 },
    { SHA3_224_LENGTH,
      sha3_224_NewContext,
      SECHash_PK11_CloneContext,
      SECHash_PK11_DestroyContext,
      SECHash_PK11_DigestBegin,
      SECHash_PK11_DigestOp,
      SECHash_PK11_DigestFinal,
      SHA3_224_BLOCK_LENGTH,
      HASH_AlgSHA3_224 },
    { SHA3_256_LENGTH,
      sha3_256_NewContext,
      SECHash_PK11_CloneContext,
      SECHash_PK11_DestroyContext,
      SECHash_PK11_DigestBegin,
      SECHash_PK11_DigestOp,
      SECHash_PK11_DigestFinal,
      SHA3_256_BLOCK_LENGTH,
      HASH_AlgSHA3_256 },
    { SHA3_384_LENGTH,
      sha3_384_NewContext,
      SECHash_PK11_CloneContext,
      SECHash_PK11_DestroyContext,
      SECHash_PK11_DigestBegin,
      SECHash_PK11_DigestOp,
      SECHash_PK11_DigestFinal,
      SHA3_384_BLOCK_LENGTH,
      HASH_AlgSHA3_384 },
    { SHA3_512_LENGTH,
      sha3_512_NewContext,
      SECHash_PK11_CloneContext,
      SECHash_PK11_DestroyContext,
      SECHash_PK11_DigestBegin,
      SECHash_PK11_DigestOp,
      SECHash_PK11_DigestFinal,
      SHA3_512_BLOCK_LENGTH,
      HASH_AlgSHA3_512 },
};

const SECHashObject *
HASH_GetHashObject(HASH_HashType type)
{
    return &SECHashObjects[type];
}

const SECHashObject *
HASH_GetHashObjectByOidTag(SECOidTag hashOid)
{
    HASH_HashType ht = HASH_GetHashTypeByOidTag(hashOid);

    return (ht == HASH_AlgNULL) ? NULL : &SECHashObjects[ht];
}

/* returns zero for unknown hash OID */
unsigned int
HASH_ResultLenByOidTag(SECOidTag hashOid)
{
    const SECHashObject *hashObject = HASH_GetHashObjectByOidTag(hashOid);
    unsigned int resultLen = 0;

    if (hashObject)
        resultLen = hashObject->length;
    return resultLen;
}

/* returns zero if hash type invalid. */
unsigned int
HASH_ResultLen(HASH_HashType type)
{
    if ((type < HASH_AlgNULL) || (type >= HASH_AlgTOTAL)) {
        PORT_SetError(SEC_ERROR_INVALID_ALGORITHM);
        return (0);
    }

    return (SECHashObjects[type].length);
}

unsigned int
HASH_ResultLenContext(HASHContext *context)
{
    return (context->hashobj->length);
}

SECStatus
HASH_HashBuf(HASH_HashType type,
             unsigned char *dest,
             const unsigned char *src,
             PRUint32 src_len)
{
    HASHContext *cx;
    unsigned int part;

    if ((type < HASH_AlgNULL) || (type >= HASH_AlgTOTAL)) {
        return (SECFailure);
    }

    cx = HASH_Create(type);
    if (cx == NULL) {
        return (SECFailure);
    }
    HASH_Begin(cx);
    HASH_Update(cx, src, src_len);
    HASH_End(cx, dest, &part, HASH_ResultLenContext(cx));
    HASH_Destroy(cx);

    return (SECSuccess);
}

HASHContext *
HASH_Create(HASH_HashType type)
{
    void *hash_context = NULL;
    HASHContext *ret = NULL;

    if ((type < HASH_AlgNULL) || (type >= HASH_AlgTOTAL)) {
        return (NULL);
    }

    hash_context = (*SECHashObjects[type].create)();
    if (hash_context == NULL) {
        goto loser;
    }

    ret = (HASHContext *)PORT_Alloc(sizeof(HASHContext));
    if (ret == NULL) {
        goto loser;
    }

    ret->hash_context = hash_context;
    ret->hashobj = &SECHashObjects[type];

    return (ret);

loser:
    if (hash_context != NULL) {
        (*SECHashObjects[type].destroy)(hash_context, PR_TRUE);
    }

    return (NULL);
}

HASHContext *
HASH_Clone(HASHContext *context)
{
    void *hash_context = NULL;
    HASHContext *ret = NULL;

    hash_context = (*context->hashobj->clone)(context->hash_context);
    if (hash_context == NULL) {
        goto loser;
    }

    ret = (HASHContext *)PORT_Alloc(sizeof(HASHContext));
    if (ret == NULL) {
        goto loser;
    }

    ret->hash_context = hash_context;
    ret->hashobj = context->hashobj;

    return (ret);

loser:
    if (hash_context != NULL) {
        (*context->hashobj->destroy)(hash_context, PR_TRUE);
    }

    return (NULL);
}

void
HASH_Destroy(HASHContext *context)
{
    (*context->hashobj->destroy)(context->hash_context, PR_TRUE);
    PORT_Free(context);
    return;
}

void
HASH_Begin(HASHContext *context)
{
    (*context->hashobj->begin)(context->hash_context);
    return;
}

void
HASH_Update(HASHContext *context,
            const unsigned char *src,
            unsigned int len)
{
    (*context->hashobj->update)(context->hash_context, src, len);
    return;
}

void
HASH_End(HASHContext *context,
         unsigned char *result,
         unsigned int *result_len,
         unsigned int max_result_len)
{
    (*context->hashobj->end)(context->hash_context, result, result_len,
                             max_result_len);
    return;
}

HASH_HashType
HASH_GetType(HASHContext *context)
{
    return (context->hashobj->type);
}
