package main

import (
	"encoding/hex"
	"fmt"
)

type AES struct {
	// 密钥长度：16, 24, 32 字节对应 AES-128, AES-192, AES-256
	key       []byte     // 密钥
	keySize   int        // 密钥长度
	Nr        int        // 轮数: 10, 12, 14 for AES-128, AES-192, AES-256
	Nb        int        // 数据块大小 (4*4字节)
	Nk        int        // 密钥长度 (4, 6, 8)
	state     [4][4]byte // 状态矩阵
	roundKeys []uint32   // 轮密钥
	sbox      [256]byte  // S盒
	invSbox   [256]byte  // 逆S盒
	rcon      [11]byte   // 轮常量
}

// NewAES 创建一个新的AES对象
func NewAES(key []byte) (*AES, error) {
	aes := &AES{
		key:     make([]byte, len(key)),
		keySize: len(key),
		Nr:      10, // 直接设定为10轮，AES-128标准
		Nb:      4,
		Nk:      4,
	}

	// 复制密钥
	copy(aes.key, key)

	// 初始化S盒和逆S盒
	aes.initSBox()

	// 初始化轮常量
	aes.initRcon()

	// 密钥扩展
	aes.keyExpansion()

	return aes, nil
}

// 初始化S盒
func (a *AES) initSBox() {
	// AES S盒的定义（预计算的结果）
	sboxValues := []byte{
		0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
		0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
		0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
		0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
		0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
		0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
		0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
		0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
		0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
		0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
		0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
		0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
		0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
		0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
		0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
		0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16,
	}

	// 逆S盒的定义（预计算的结果）
	invSboxValues := []byte{
		0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
		0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
		0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
		0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
		0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
		0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
		0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
		0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
		0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
		0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
		0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
		0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
		0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
		0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
		0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
		0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d,
	}

	copy(a.sbox[:], sboxValues)
	copy(a.invSbox[:], invSboxValues)
}

// 初始化轮常量
func (a *AES) initRcon() {
	a.rcon = [11]byte{0x00, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36}
}

// 密钥扩展
func (a *AES) keyExpansion() {
	// 计算轮密钥总数
	a.roundKeys = make([]uint32, 4*a.Nb*(a.Nr+1))

	// 复制原始密钥到轮密钥的前面部分
	for i := range a.Nk {
		a.roundKeys[i] = uint32(a.key[4*i])<<24 |
			uint32(a.key[4*i+1])<<16 |
			uint32(a.key[4*i+2])<<8 |
			uint32(a.key[4*i+3])
	}

	// 扩展剩余的轮密钥
	for i := a.Nk; i < a.Nb*(a.Nr+1); i++ {
		temp := a.roundKeys[i-1]

		if i%a.Nk == 0 {
			// 对每Nk个字进行特殊处理
			// 循环左移一个字节
			temp = (temp << 8) | (temp >> 24)

			// 通过S盒进行替换
			temp = uint32(a.sbox[byte(temp>>24)])<<24 |
				uint32(a.sbox[byte(temp>>16)])<<16 |
				uint32(a.sbox[byte(temp>>8)])<<8 |
				uint32(a.sbox[byte(temp)])

			// 与轮常量异或
			temp ^= uint32(a.rcon[i/a.Nk]) << 24
		} else if a.Nk > 6 && i%a.Nk == 4 {
			// AES-256额外的S盒替换
			temp = uint32(a.sbox[byte(temp>>24)])<<24 |
				uint32(a.sbox[byte(temp>>16)])<<16 |
				uint32(a.sbox[byte(temp>>8)])<<8 |
				uint32(a.sbox[byte(temp)])
		}

		a.roundKeys[i] = a.roundKeys[i-a.Nk] ^ temp
	}
}

// 将字节数组转换为状态矩阵
func (a *AES) bytesToState(in []byte) {
	for i := 0; i < 4; i++ {
		for j := 0; j < 4; j++ {
			a.state[j][i] = in[i*4+j]
		}
	}
}

// 将状态矩阵转换为字节数组
func (a *AES) stateToBytes() []byte {
	out := make([]byte, 16)
	for i := 0; i < 4; i++ {
		for j := 0; j < 4; j++ {
			out[i*4+j] = a.state[j][i]
		}
	}
	return out
}

// GF(2^8)上的乘法
func (a *AES) gmul(x, y byte) byte {
	var product byte = 0
	var highBitSet byte

	for i := 0; i < 8; i++ {
		if (y & 1) != 0 {
			product ^= x
		}

		highBitSet = x & 0x80
		x <<= 1

		if highBitSet != 0 {
			x ^= 0x1b // AES中的不可约多项式 x^8 + x^4 + x^3 + x + 1
		}

		y >>= 1
	}

	return product
}

// 子字节变换 (SubBytes)
func (a *AES) subBytes() {
	for i := 0; i < 4; i++ {
		for j := 0; j < 4; j++ {
			a.state[i][j] = a.sbox[a.state[i][j]]
		}
	}
}

// 行移位 (ShiftRows)
func (a *AES) shiftRows() {
	var temp [4]byte

	// 第一行不移位

	// 第二行左移一位
	temp[0] = a.state[1][0]
	a.state[1][0] = a.state[1][1]
	a.state[1][1] = a.state[1][2]
	a.state[1][2] = a.state[1][3]
	a.state[1][3] = temp[0]

	// 第三行左移两位
	temp[0] = a.state[2][0]
	temp[1] = a.state[2][1]
	a.state[2][0] = a.state[2][2]
	a.state[2][1] = a.state[2][3]
	a.state[2][2] = temp[0]
	a.state[2][3] = temp[1]

	// 第四行左移三位
	temp[0] = a.state[3][3]
	a.state[3][3] = a.state[3][2]
	a.state[3][2] = a.state[3][1]
	a.state[3][1] = a.state[3][0]
	a.state[3][0] = temp[0]
}

// 列混合 (MixColumns)
func (a *AES) mixColumns() {
	var temp [4]byte

	for i := 0; i < 4; i++ {
		temp[0] = a.state[0][i]
		temp[1] = a.state[1][i]
		temp[2] = a.state[2][i]
		temp[3] = a.state[3][i]

		a.state[0][i] = a.gmul(temp[0], 2) ^ a.gmul(temp[1], 3) ^ temp[2] ^ temp[3]
		a.state[1][i] = temp[0] ^ a.gmul(temp[1], 2) ^ a.gmul(temp[2], 3) ^ temp[3]
		a.state[2][i] = temp[0] ^ temp[1] ^ a.gmul(temp[2], 2) ^ a.gmul(temp[3], 3)
		a.state[3][i] = a.gmul(temp[0], 3) ^ temp[1] ^ temp[2] ^ a.gmul(temp[3], 2)
	}
}

// 将轮密钥添加到状态矩阵 (AddRoundKey)
func (a *AES) addRoundKey(round int) {
	for i := 0; i < 4; i++ {
		for j := 0; j < 4; j++ {
			a.state[j][i] ^= byte(a.roundKeys[round*4+i] >> uint(24-j*8))
		}
	}
}


// 逆子字节变换 (InvSubBytes)
func (a *AES) invSubBytes() {
	for i := 0; i < 4; i++ {
		for j := 0; j < 4; j++ {
			a.state[i][j] = a.invSbox[a.state[i][j]]
		}
	}
}

// 逆行移位 (InvShiftRows)
func (a *AES) invShiftRows() {
	var temp [4]byte

	// 第一行不移位

	// 第二行右移一位
	temp[0] = a.state[1][3]
	a.state[1][3] = a.state[1][2]
	a.state[1][2] = a.state[1][1]
	a.state[1][1] = a.state[1][0]
	a.state[1][0] = temp[0]

	// 第三行右移两位
	temp[0] = a.state[2][0]
	temp[1] = a.state[2][1]
	a.state[2][0] = a.state[2][2]
	a.state[2][1] = a.state[2][3]
	a.state[2][2] = temp[0]
	a.state[2][3] = temp[1]

	// 第四行右移三位
	temp[0] = a.state[3][0]
	a.state[3][0] = a.state[3][1]
	a.state[3][1] = a.state[3][2]
	a.state[3][2] = a.state[3][3]
	a.state[3][3] = temp[0]
}

// 逆列混合 (InvMixColumns)
func (a *AES) invMixColumns() {
	var temp [4]byte

	for i := 0; i < 4; i++ {
		temp[0] = a.state[0][i]
		temp[1] = a.state[1][i]
		temp[2] = a.state[2][i]
		temp[3] = a.state[3][i]

		a.state[0][i] = a.gmul(temp[0], 0x0e) ^ a.gmul(temp[1], 0x0b) ^ a.gmul(temp[2], 0x0d) ^ a.gmul(temp[3], 0x09)
		a.state[1][i] = a.gmul(temp[0], 0x09) ^ a.gmul(temp[1], 0x0e) ^ a.gmul(temp[2], 0x0b) ^ a.gmul(temp[3], 0x0d)
		a.state[2][i] = a.gmul(temp[0], 0x0d) ^ a.gmul(temp[1], 0x09) ^ a.gmul(temp[2], 0x0e) ^ a.gmul(temp[3], 0x0b)
		a.state[3][i] = a.gmul(temp[0], 0x0b) ^ a.gmul(temp[1], 0x0d) ^ a.gmul(temp[2], 0x09) ^ a.gmul(temp[3], 0x0e)
	}
}

// 加密一个16字节的块
func (a *AES) encryptBlock(input []byte) []byte {
	if len(input) != 16 {
		panic("AES块大小必须是16字节")
	}

	// 将输入转换为状态矩阵
	a.bytesToState(input)

	// 初始轮密钥加
	a.addRoundKey(0)

	// 主加密循环
	for round := 1; round < a.Nr; round++ {
		a.subBytes()
		a.shiftRows()
		a.mixColumns()
		a.addRoundKey(round)
	}

	// 最后一轮（没有MixColumns）
	a.subBytes()
	a.shiftRows()
	a.addRoundKey(a.Nr)

	// 将状态矩阵转换回字节数组
	return a.stateToBytes()
}

// 解密一个16字节的块
func (a *AES) decryptBlock(input []byte) []byte {
	if len(input) != 16 {
		panic("AES块大小必须是16字节")
	}

	// 将输入转换为状态矩阵
	a.bytesToState(input)

	// 初始轮密钥加（使用最后一轮的密钥）
	a.addRoundKey(a.Nr)

	// 主解密循环
	for round := a.Nr - 1; round > 0; round-- {
		a.invShiftRows()
		a.invSubBytes()
		a.addRoundKey(round)
		a.invMixColumns()
	}

	// 最后一轮（没有逆列混合）
	a.invShiftRows()
	a.invSubBytes()
	a.addRoundKey(0)

	// 将状态矩阵转换回字节数组
	return a.stateToBytes()
}

// // PKCS#7填充
// func pkcs7Pad(data []byte, blockSize int) []byte {
// 	padding := blockSize - len(data)%blockSize
// 	padText := bytes.Repeat([]byte{byte(padding)}, padding)
// 	return append(data, padText...)
// }

// // 移除PKCS#7填充
// func pkcs7Unpad(data []byte) []byte {
// 	length := len(data)
// 	if length == 0 {
// 		return nil
// 	}

// 	padding := int(data[length-1])
// 	if padding > length {
// 		return nil // 无效的填充
// 	}

// 	return data[:length-padding]
// }

// Encrypt 加密数据
func (a *AES) Encrypt(plaintext []byte) ([]byte, error) {
	// // 使用PKCS#7填充
	// plaintext = pkcs7Pad(plaintext, 16)

	// 存储加密后的数据
	ciphertext := make([]byte, len(plaintext))

	// 分块加密
	for i := 0; i < len(plaintext); i += 16 {
		copy(ciphertext[i:i+16], a.encryptBlock(plaintext[i:i+16]))
	}

	return ciphertext, nil
}

// Decrypt 解密数据
func (a *AES) Decrypt(ciphertext []byte) ([]byte, error) {
	if len(ciphertext)%16 != 0 {
		return nil, fmt.Errorf("密文长度必须是16的倍数")
	}

	// 存储解密后的数据
	plaintext := make([]byte, len(ciphertext))

	// 分块解密
	for i := 0; i < len(ciphertext); i += 16 {
		copy(plaintext[i:i+16], a.decryptBlock(ciphertext[i:i+16]))
	}

	// 移除填充
	return plaintext, nil
}

// Cipher 加密器接口
type Cipher interface {
	Encrypt(plaintext []byte) ([]byte, error)
	Decrypt(ciphertext []byte) ([]byte, error)
}

// CryptoService 加密服务
type CryptoService struct {
	cipher Cipher
	key    []byte
}

// NewCryptoService 创建一个新的加密服务
func NewCryptoService(key []byte) (*CryptoService, error) {
	if len(key) != 16 {
		return nil, fmt.Errorf("AES-128实现仅支持16字节密钥，传入了: %d", len(key))
	}

	cipher, err := NewAES(key)
	if err != nil {
		return nil, err
	}

	return &CryptoService{
		cipher: cipher,
		key:    key,
	}, nil
}

// GetKey 获取当前使用的密钥
func (s *CryptoService) GetKey() []byte {
	return s.key
}

// EncryptData 加密数据并返回十六进制字符串
func (s *CryptoService) EncryptData(data []byte) (string, error) {
	encrypted, err := s.cipher.Encrypt(data)
	if err != nil {
		return "", err
	}
	return hex.EncodeToString(encrypted), nil
}

// DecryptData 解密十六进制字符串数据
func (s *CryptoService) DecryptData(hexData string) ([]byte, error) {
	data, err := hex.DecodeString(hexData)
	if err != nil {
		return nil, err
	}
	return s.cipher.Decrypt(data)
}

func main() {
	// 使用固定的密钥 (按列读取矩阵)
	// 2b 28 ab 09
	// 7e ae f7 cf
	// 15 d2 15 4f
	// 16 a6 88 3c
	key := []byte{
		0x2b, 0x7e, 0x15, 0x16, // 第一列
		0x28, 0xae, 0xd2, 0xa6, // 第二列
		0xab, 0xf7, 0x15, 0x88, // 第三列
		0x09, 0xcf, 0x4f, 0x3c, // 第四列
	}

	// 创建一个AES-128加密实例
	aes, err := NewAES(key)
	if err != nil {
		fmt.Printf("创建AES实例错误: %v\n", err)
		return
	}

	// 使用固定的明文 (按列读取矩阵)
	// 32 88 31 e0
	// 43 5a 31 37
	// f6 30 98 07
	// 18 8d a2 34
	plaintext := []byte{
		0x32, 0x43, 0xf6, 0x18, // 第一列
		0x88, 0x5a, 0x30, 0x8d, // 第二列
		0x31, 0x31, 0x98, 0xa2, // 第三列
		0xe0, 0x37, 0x07, 0x34, // 第四列
	}

	fmt.Println("原始明文矩阵:")
	printMatrix(plaintext)
	fmt.Println("密钥矩阵:")
	printMatrix(key)

	// 直接加密
	ciphertext, err := aes.Encrypt(plaintext)
	if err != nil {
		fmt.Printf("加密错误: %v\n", err)
		return
	}

	// 转换为十六进制显示
	hexCiphertext := hex.EncodeToString(ciphertext)
	fmt.Println("加密后(hex):", hexCiphertext)
	fmt.Println("加密后矩阵:")
	printMatrix(ciphertext)

	// 解密
	decrypted, err := aes.Decrypt(ciphertext)
	if err != nil {
		fmt.Printf("解密错误: %v\n", err)
		return
	}
	fmt.Println("解密后:", hex.EncodeToString(decrypted))
	fmt.Println("解密后矩阵:")
	printMatrix(decrypted)
}

// 辅助函数：打印4x4矩阵
// TODO: 最后可以删除
func printMatrix(data []byte) {
	if len(data) < 16 {
		fmt.Println("数据长度不足")
		return
	}

	for i := 0; i < 4; i++ {
		for j := 0; j < 4; j++ {
			fmt.Printf("%02x ", data[j*4+i])
		}
		fmt.Println()
	}
}
