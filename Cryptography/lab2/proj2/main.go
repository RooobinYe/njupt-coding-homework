package main

import (
	"bytes"
	"crypto/aes"
	"crypto/cipher"
	"crypto/rand"
	"encoding/hex"
	"fmt"
	"io"

	"github.com/tjfoc/gmsm/sm4"
)

// AES加密函数
func aesEncrypt(plaintext []byte, key []byte) ([]byte, error) {
	// 创建加密块
	block, err := aes.NewCipher(key)
	if err != nil {
		return nil, err
	}

	// 填充明文使其长度为16的倍数
	padding := aes.BlockSize - (len(plaintext) % aes.BlockSize)
	padText := bytes.Repeat([]byte{byte(padding)}, padding)
	paddedPlaintext := append(plaintext, padText...)

	// 创建随机IV
	ciphertext := make([]byte, aes.BlockSize+len(paddedPlaintext))
	iv := ciphertext[:aes.BlockSize]
	if _, err := io.ReadFull(rand.Reader, iv); err != nil {
		return nil, err
	}

	// 加密
	mode := cipher.NewCBCEncrypter(block, iv)
	mode.CryptBlocks(ciphertext[aes.BlockSize:], paddedPlaintext)

	return ciphertext, nil
}

// AES解密函数
func aesDecrypt(ciphertext []byte, key []byte) ([]byte, error) {
	// 创建解密块
	block, err := aes.NewCipher(key)
	if err != nil {
		return nil, err
	}

	// 检查密文长度
	if len(ciphertext) < aes.BlockSize {
		return nil, fmt.Errorf("密文太短")
	}

	// 提取IV
	iv := ciphertext[:aes.BlockSize]
	ciphertext = ciphertext[aes.BlockSize:]

	// 解密
	mode := cipher.NewCBCDecrypter(block, iv)
	plaintext := make([]byte, len(ciphertext))
	mode.CryptBlocks(plaintext, ciphertext)

	// 去除填充
	padding := int(plaintext[len(plaintext)-1])
	return plaintext[:len(plaintext)-padding], nil
}

// SM4加密函数
func sm4Encrypt(plaintext []byte, key []byte) ([]byte, error) {
	// 创建加密块
	block, err := sm4.NewCipher(key)
	if err != nil {
		return nil, err
	}

	// 填充明文使其长度为16的倍数
	padding := sm4.BlockSize - (len(plaintext) % sm4.BlockSize)
	padText := bytes.Repeat([]byte{byte(padding)}, padding)
	paddedPlaintext := append(plaintext, padText...)

	// 创建随机IV
	ciphertext := make([]byte, sm4.BlockSize+len(paddedPlaintext))
	iv := ciphertext[:sm4.BlockSize]
	if _, err := io.ReadFull(rand.Reader, iv); err != nil {
		return nil, err
	}

	// 加密
	mode := cipher.NewCBCEncrypter(block, iv)
	mode.CryptBlocks(ciphertext[sm4.BlockSize:], paddedPlaintext)

	return ciphertext, nil
}

// SM4解密函数
func sm4Decrypt(ciphertext []byte, key []byte) ([]byte, error) {
	// 创建解密块
	block, err := sm4.NewCipher(key)
	if err != nil {
		return nil, err
	}

	// 检查密文长度
	if len(ciphertext) < sm4.BlockSize {
		return nil, fmt.Errorf("密文太短")
	}

	// 提取IV
	iv := ciphertext[:sm4.BlockSize]
	ciphertext = ciphertext[sm4.BlockSize:]

	// 解密
	mode := cipher.NewCBCDecrypter(block, iv)
	plaintext := make([]byte, len(ciphertext))
	mode.CryptBlocks(plaintext, ciphertext)

	// 去除填充
	padding := int(plaintext[len(plaintext)-1])
	return plaintext[:len(plaintext)-padding], nil
}

func main() {
	// 示例明文
	plaintext := []byte("这是一段需要加密的文本")
	fmt.Println("原始文本:", string(plaintext))

	// AES加解密示例
	fmt.Println("\nAES加解密:")
	// AES-128需要16字节密钥
	aesKey := []byte("0123456789abcdef")
	fmt.Println("AES密钥:", hex.EncodeToString(aesKey))

	// 加密
	aesCiphertext, err := aesEncrypt(plaintext, aesKey)
	if err != nil {
		fmt.Println("AES加密错误:", err)
		return
	}
	fmt.Println("AES加密后:", hex.EncodeToString(aesCiphertext))

	// 解密
	aesDecryptedText, err := aesDecrypt(aesCiphertext, aesKey)
	if err != nil {
		fmt.Println("AES解密错误:", err)
		return
	}
	fmt.Println("AES解密后:", string(aesDecryptedText))

	// SM4加解密示例
	fmt.Println("\nSM4加解密:")
	// SM4需要16字节密钥
	sm4Key := []byte("0123456789abcdef")
	fmt.Println("SM4密钥:", hex.EncodeToString(sm4Key))

	// 加密
	sm4Ciphertext, err := sm4Encrypt(plaintext, sm4Key)
	if err != nil {
		fmt.Println("SM4加密错误:", err)
		return
	}
	fmt.Println("SM4加密后:", hex.EncodeToString(sm4Ciphertext))

	// 解密
	sm4DecryptedText, err := sm4Decrypt(sm4Ciphertext, sm4Key)
	if err != nil {
		fmt.Println("SM4解密错误:", err)
		return
	}
	fmt.Println("SM4解密后:", string(sm4DecryptedText))
}
