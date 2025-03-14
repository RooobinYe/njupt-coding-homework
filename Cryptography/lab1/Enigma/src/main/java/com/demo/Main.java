package com.demo;

public class Main {
    // 字母频率分析方法
    private static void analyzeFrequency(String text) {
        int[] frequency = new int[26];
        text = text.toUpperCase();
        
        // 统计每个字母出现的次数
        for (char c : text.toCharArray()) {
            if (c >= 'A' && c <= 'Z') {
                frequency[c - 'A']++;
            }
        }
        
        // 计算总字母数
        int total = 0;
        for (int count : frequency) {
            total += count;
        }
        
        // 输出每个字母的频率
        System.out.println("字母频率分析：");
        for (int i = 0; i < 26; i++) {
            if (frequency[i] > 0) {
                double percentage = (frequency[i] * 100.0) / total;
                System.out.printf("%c: %.2f%% (%d次)%n", 
                    (char)('A' + i), percentage, frequency[i]);
            }
        }
        System.out.println();
    }

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
        System.out.println("原文字母频率分析：");
        analyzeFrequency(plaintext);

        String ciphertext = enc2.encrypt(plaintext);
        System.out.println("密文: " + ciphertext);
        System.out.println();
        System.out.println("密文字母频率分析：");
        analyzeFrequency(ciphertext);
        
        String decrypted = dec2.decrypt(ciphertext);
        System.out.println("解密: " + decrypted);
        System.out.println();
    }
}