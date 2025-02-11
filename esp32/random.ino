String generateRandomString(int length) {
    String randomString = "";
    const char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    int charsetSize = sizeof(charset) - 1;

    randomSeed(analogRead(0)); 
    for (int i = 0; i < length; i++) {
        randomString += charset[random(charsetSize)];
    }

    return randomString;
}