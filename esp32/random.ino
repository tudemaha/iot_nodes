#include <random>
#include <algorithm>

String generateRandomString(size_t length) {
  const String characters = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
  std::random_device random_device;
  std::mt19937 generator(random_device());

  String random_string = characters;
  std::shuffle(random_string.begin(), random_string.end(), generator);

  return random_string.substring(0, length);
}