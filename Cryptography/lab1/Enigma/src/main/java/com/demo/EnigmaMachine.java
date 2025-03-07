package com.demo;

import java.util.*;

public class EnigmaMachine {
    // 转子，用于字符替换和位置旋转 eg: {"I", "II", "III"}
    private Rotor[] rotors;
    // 反射器，用于将信号反射回转子
    private Reflector reflector;
    // 插线板，用于初始字符对替换
    private Plugboard plugboard;
    // 记录转子旋转次数的计数器
    private int[] rotationCounters;

    /**
     * 构造 Enigma 加密机
     *
     * @param rotorTypes           选择的转子类型（I-V）
     * @param reflectorType        选择的反射器类型（A-C）
     * @param initialPositions     转子的初始位置 eg: {A, A, A} // (0, 0, 0)
     * @param plugboardConnections 插线板连接
     */
    public EnigmaMachine(String[] rotorTypes, String reflectorType, int[] initialPositions,
            String[] plugboardConnections) {
        // 验证参数
        if (rotorTypes.length != initialPositions.length) {
            throw new IllegalArgumentException("转子数量与初始位置数量不匹配");
        }

        this.rotors = new Rotor[rotorTypes.length];
        for (int i = 0; i < rotorTypes.length; i++) {
            this.rotors[i] = new Rotor(rotorTypes[i]);
            this.rotors[i].setPosition(initialPositions[i]);
        }
        this.reflector = new Reflector(reflectorType);
        this.plugboard = new Plugboard(plugboardConnections);
        
        // 初始化旋转计数器
        this.rotationCounters = new int[rotorTypes.length];
    }

    /**
     * 加密/解密一个字符
     *
     * @param c 需要加密/解密的字符
     * @return 加密/解密后的字符
     */
    public char process(char c) {
        // 排除了非字母字符
        if (!Character.isLetter(c)) {
            return c;
        }

        // 旋转转子（按照先右后左的顺序）
        rotateRotors();

        // 通过插线板
        c = plugboard.forward(c);

        // 从右到左通过转子（加密路径）
        for (int i = rotors.length - 1; i >= 0; i--) {
            c = rotors[i].forward(c);
        }

        // 通过反射器
        c = reflector.reflect(c);

        // 从左到右通过转子（解密路径）
        for (int i = 0; i < rotors.length; i++) {
            c = rotors[i].backward(c);
        }

        // 再次通过插线板
        c = plugboard.forward(c);

        return c;
    }

    /**
     * 处理整个字符串
     *
     * @param text 需要处理的文本
     * @return 处理后的文本
     */
    public String process(String text) {
        StringBuilder result = new StringBuilder();
        for (char c : text.toCharArray()) {
            result.append(process(c));
        }
        return result.toString();
    }

    /**
     * 旋转转子（按照时钟机制）
     * 第一个转子每次都转
     * 第二个转子在第一个转了26下转一下
     * 第三个转子在第二个转了26下转一下
     */
    private void rotateRotors() {
        // 确保数组已经初始化
        if (rotationCounters == null) {
            rotationCounters = new int[rotors.length];
        }
        
        // 第一个转子（最右侧）总是旋转
        rotors[0].rotate();
        rotationCounters[0]++;
        
        // 检查是否需要旋转第二个转子
        if (rotationCounters[0] >= 26) {
            rotationCounters[0] = 0; // 重置计数器
            if (rotors.length > 1) {
                rotors[1].rotate();
                rotationCounters[1]++;
                
                // 检查是否需要旋转第三个转子
                if (rotationCounters[1] >= 26) {
                    rotationCounters[1] = 0;
                    if (rotors.length > 2) {
                        rotors[2].rotate();
                        // 如果有第四个转子，可以按照相同的逻辑继续扩展
                    }
                }
            }
        }
    }

    /**
     * 转子类，实现字符替换和旋转功能
     */
    private static class Rotor {
        // 英文字母表
        private static final String ALPHABET = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

        // 转子的接线方式
        private String wiring;
        // 转子当前位置 (0-25)
        private int position;

        /**
         * 根据转子类型初始化转子
         *
         * @param type 转子类型（I-V）
         */
        public Rotor(String type) {
            switch (type) {
                case "I":
                    wiring = "UCOASJNZTHPGVDKEQILWRBYFXM";
                    break;
                case "II":
                    wiring = "TAFDOCNLWEPBVSKRYXMGJHUIZQ";
                    break;
                case "III":
                    wiring = "NHRZQTVJCMKDWEXILYPSFOUBGA";
                    break;
                // case "IV":
                //     wiring = "ESOVPZJAYQUIRHXLNFTGKDCMWB";
                //     break;
                // case "V":
                //     wiring = "VZBRGITYUPSDNHLXAWMJQOFECK";
                //     break;
                default:
                    throw new IllegalArgumentException("不支持的转子类型: " + type);
            }
            position = 0; // 初始位置为 0
        }

        /**
         * 设置转子位置
         *
         * @param pos 位置值 (0-25)
         */
        public void setPosition(int pos) {
            this.position = pos % 26;
        }

        /**
         * 转子旋转一步
         */
        public void rotate() {
            position = (position + 1) % 26;
        }

        /**
         * 正向通过转子（右→左）
         *
         * @param c 输入字符
         * @return 输出字符
         */
        public char forward(char c) {
            int index = (ALPHABET.indexOf(c) + position) % 26;
            char result = wiring.charAt(index);
            return ALPHABET.charAt((ALPHABET.indexOf(result) - position + 26) % 26);
        }

        /**
         * 反向通过转子（左→右）
         *
         * @param c 输入字符
         * @return 输出字符
         */
        public char backward(char c) {
            int index = (ALPHABET.indexOf(c) + position) % 26;
            int resultIndex = wiring.indexOf(ALPHABET.charAt(index));
            return ALPHABET.charAt((resultIndex - position + 26) % 26);
        }
    }

    /**
     * 反射器类，将信号反射回转子系统
     */
    private static class Reflector {
        private Map<Character, Character> wiring;

        /**
         * 根据反射器类型初始化
         *
         * @param type 反射器类型（A-C）
         */
        public Reflector(String type) {
            wiring = new HashMap<>();
            String wiringStr;

            switch (type) {
                case "A":
                    wiringStr = "EJMZALYXVBWFCRQUONTSPIKHGD";
                    break;
                case "B":
                    wiringStr = "YRUHQSLDPXNGOKMIEBFZCWVJAT";
                    break;
                case "C":
                    wiringStr = "FVPJIAOYEDRZXWGCTKUQSBNMHL";
                    break;
                default:
                    throw new IllegalArgumentException("不支持的反射器类型: " + type);
            }

            // 构建反射映射
            for (int i = 0; i < 26; i++) {
                char a = (char) ('A' + i);
                char b = wiringStr.charAt(i);
                wiring.put(a, b);
                wiring.put(b, a);
            }
        }

        /**
         * 反射一个字符
         *
         * @param c 输入字符
         * @return 反射后的字符
         */
        public char reflect(char c) {
            return wiring.getOrDefault(c, c);
        }
    }

    /**
     * 插线板类，实现字符对交换
     */
    private static class Plugboard {
        private Map<Character, Character> wiring;

        /**
         * 初始化插线板
         *
         * @param connections 连接字符对（例如："AB CD"）
         */
        public Plugboard(String[] connections) {
            wiring = new HashMap<>();

            // 没有连接时，字符映射到自身
            for (char c = 'A'; c <= 'Z'; c++) {
                wiring.put(c, c);
            }

            // 添加连接
            if (connections != null) {
                for (String connection : connections) {
                    if (connection.length() >= 2) {
                        char a = connection.charAt(0);
                        char b = connection.charAt(1);
                        wiring.put(a, b);
                        wiring.put(b, a);
                    }
                }
            }
        }

        /**
         * 通过插线板
         *
         * @param c 输入字符
         * @return 输出字符
         */
        public char forward(char c) {
            return wiring.getOrDefault(Character.toUpperCase(c), c);
        }
    }
}