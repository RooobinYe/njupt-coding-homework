package com.demo;

public class Main {
    public static void main(String[] args) {
        String plaintext = "the state key laboratory of networking and switching technology belongs to beijing university of posts and telecommunication the laboratory was opened in nineteen ninety two in nineteen ninety five the laboratory passed acceptance inspection by government and an evaluation organized by ministry of science and technology in two thousand and two since two thousand and four the laboratory has been renamed as the state key laboratory of networking and switching technology by ministry of science and technology";

        System.out.println("Enigma 加密机演示");
        System.out.println("------------------");
        
        // 示例 1：使用自定义配置
        System.out.println("示例 1: 使用自定义配置");
        String[] rotors = {"I", "II", "III"};
        String reflector = "B";
        String initialPosition = "XZZ";
        // String[] plugboard = {"AB", "CD", "EF"};
        String[] plugboard = null;
        
        Encryption enc2 = new Encryption(rotors, reflector, initialPosition, plugboard);
        Decryption dec2 = new Decryption(rotors, reflector, initialPosition, plugboard);
        
        System.out.println("原文: " + plaintext);
        System.out.println();

        String ciphertext = enc2.encrypt(plaintext);
        System.out.println("密文: " + ciphertext);
        System.out.println();

        
        String decrypted = dec2.decrypt(ciphertext);
        System.out.println("解密: " + decrypted);
        System.out.println();
        
        // // 示例 3：处理包含空格和非字母字符的文本
        // System.out.println("示例 3: 处理包含空格和非字母字符的文本");
        // String[] rotors3 = {"V", "III", "I"};
        // String reflector3 = "C";
        // int[] positions3 = {7, 11, 19}; // H, L, T
        
        // Encryption enc3 = new Encryption(rotors3, reflector3, positions3, null);
        // Decryption dec3 = new Decryption(rotors3, reflector3, positions3, null);
        
        // // plaintext = "Enigma Machine - Second World War encryption device!";
        // System.out.println("原文: " + plaintext);
        
        // ciphertext = enc3.encrypt(plaintext);
        // System.out.println("密文: " + ciphertext);
        
        // decrypted = dec3.decrypt(ciphertext);
        // System.out.println("解密: " + decrypted);
    }
}