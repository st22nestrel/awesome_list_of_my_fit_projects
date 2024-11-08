# KRY project 2 - Hybrid encryption
# Timotej Ponek, xponek00
from rsa import common, transform, core
from secrets import token_bytes

def signRSA(padded_message: bytes, priv_key) -> bytes:
    keylength = common.byte_size(priv_key.n)

    payload = transform.bytes2int(padded_message)
    encrypted = core.encrypt_int(payload, priv_key.d, priv_key.n)
    block = transform.int2bytes(encrypted, keylength)

    return block

def verifyRSA(crypto: bytes, pub_key, rnd_nmb_pad, hash: bytes) -> bytes:
    blocksize = common.byte_size(pub_key.n)

    encrypted = transform.bytes2int(crypto)
    decrypted = core.decrypt_int(encrypted, pub_key.e, pub_key.n)
    cleartext = transform.int2bytes(decrypted, blocksize)
    decr_hash = cleartext[-16:]

    return decr_hash == hash

def encryptRSA(padded_message: bytes, pub_key) -> bytes:
    keylength = common.byte_size(pub_key.n)

    payload = transform.bytes2int(padded_message)
    encrypted = core.encrypt_int(payload, pub_key.e, pub_key.n)
    block = transform.int2bytes(encrypted, keylength)

    return block

def decryptRSA(crypto: bytes, priv_key, rnd_nmb_pad) -> bytes:
    blocksize = common.byte_size(priv_key.n)

    encrypted = transform.bytes2int(crypto)
    decrypted = core.decrypt_int(encrypted, priv_key.d, priv_key.n)
    cleartext = transform.int2bytes(decrypted, blocksize)

    return cleartext[240 - rnd_nmb_pad: - (rnd_nmb_pad)]

def paddingForAsymetric(message: bytes, to_pad: int, n: int) -> bytes:
    while True:
        padded = token_bytes(240 - to_pad) + message + token_bytes(to_pad)

        padded_len = transform.bytes2int(padded)
        if padded_len < n:
            break

    return padded