package com.demo;

/**
 * Enigma 加密类
 * 负责使用 EnigmaMachine 执行加密操作
 */
public class Encryption {
    private EnigmaMachine enigmaMachine;
    
    /**
     * 创建默认配置的 Enigma 加密机
     */
    public Encryption() {
        // 默认使用三个转子 I, II, III，反射器 B，初始位置 A, A, A，无插线板连接
        String[] rotors = {"I", "II", "III"};
        int[] positions = {0, 0, 0}; // A, A, A (0, 0, 0)
        enigmaMachine = new EnigmaMachine(rotors, "B", positions, null);
    }
    
    /**
     * 使用自定义设置创建 Enigma 加密机
     * 
     * @param rotorTypes 转子类型
     * @param reflectorType 反射器类型
     * @param initialPositions 初始位置
     * @param plugboardConnections 插线板连接
     */
    public Encryption(String[] rotorTypes, String reflectorType, int[] initialPositions, String[] plugboardConnections) {
        enigmaMachine = new EnigmaMachine(rotorTypes, reflectorType, initialPositions, plugboardConnections);
    }
    
    /**
     * 从字母位置创建 Enigma 加密机
     * 
     * @param rotorTypes 转子类型
     * @param reflectorType 反射器类型
     * @param initialLetters 初始字母位置（如 "AAA"）
     * @param plugboardConnections 插线板连接
     */
    public Encryption(String[] rotorTypes, String reflectorType, String initialLetters, String[] plugboardConnections) {
        int[] positions = new int[initialLetters.length()];
        for (int i = 0; i < initialLetters.length(); i++) {
            positions[i] = initialLetters.charAt(i) - 'A';
        }
        enigmaMachine = new EnigmaMachine(rotorTypes, reflectorType, positions, plugboardConnections);
    }
    
    /**
     * 加密文本
     * 
     * @param plaintext 明文
     * @return 密文
     */
    public String encrypt(String plaintext) {
        return enigmaMachine.process(plaintext);
    }
    
    /**
     * 获取当前使用的 Enigma 加密机
     * 
     * @return Enigma 加密机实例
     */
    public EnigmaMachine getEnigmaMachine() {
        return enigmaMachine;
    }
}
