#include <string>
#include <vector>

#define SQLFILENAME "ChatGenerator.db"

using _vec = std::vector<std::string>;

void SendChatContent(std::string exeName, _vec content, size_t count);
_vec ReadSQL();
void WriteSQL(_vec content);