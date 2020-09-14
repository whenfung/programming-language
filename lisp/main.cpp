/*
	author: jiang
	date: 2019-06-18
	description: 实现简易的 lisp 解释器
*/

#include <iostream>
#include <string>
#include <map>

/*********************** 全局变量 **************************/
struct var {  
	unsigned int ope;      // 标识号，用来给 run 函数识别
	std::string type;      // 标识符类型  
	std::string explain;   // 标识符对应内容
	var() : ope(0){};
	var(int ope, std::string type, std::string explain) : ope(ope), type(type), explain(explain){}
	var(var& other) : ope(other.ope), type(other.type), explain(other.explain){}
};
std::map<std::string, var> identifier_var;  // 记录一次运行中绑定的变量
std::string current_type;                   // 记录当前命令行的类型

/*********************** 函数声明 **************************/

void init();                         // 初始化解释器环境
std::string format(std::string);     // 格式化输入字符 
unsigned int extract_brackets(std::string, unsigned int);  // 将某个括号匹配并提取
bool syntax_check(std::string);      // 语法检测
std::string interpret(std::string);   // 解释命令行，返回结果
bool define(std::string);             // 单独处理 define 语句        
std::string run(std::string);         // 执行最小括号里的函数
std::string cond(std::string);        // 处理 cond 表达式
std::string handle_cond(std::string); // 处理 cond 语句
std::string atom(std::string);        // 处理原子表达式
std::string general_expression(std::string);  // 处理普通表达式

/*********************** 函数定义 **************************/

void init() {  // 题目提供的原子函数，也就是本身就有的
	identifier_var["+"]     = var(1, "Fun",     "usage : (+ x y)");
	identifier_var["-"]     = var(2, "Fun",     "usage : (- x y)");
	identifier_var["*"]     = var(3, "Fun",     "usage : (* x y)");
	identifier_var["/"]     = var(4, "Fun",     "usage : (/ x y)");
	identifier_var["eq?"]   = var(5, "Fun",     "usage : (eq？x y)");
	identifier_var["True"]  = var(6, "Boolean", "True");
	identifier_var["False"] = var(7, "Boolean", "False");
	identifier_var["cond"] = var(8, "Fun", "usage : (cond x y)");
}

std::string format(std::string cmd) {  //格式化代码
	unsigned int i = 0;       
	std::string cmd1;
	while (i < cmd.length()) {  // 处理开头空格
		if (cmd[i] != ' ') break;
		i++;
	}
	while (i < cmd.length()) {
		if (cmd[i] == ' ') {
			if (cmd[i - 1] == '(' || (i+1 < cmd.length() && cmd[i+1] == ')' || cmd[i-1] == ' ')) {
				i++;
				continue;
			}
		}
		if (cmd[i] == ' ' && i == cmd.length() - 1) break;  // 命令行后面空格
		cmd1 += cmd[i];
		i++;
	}
	return cmd1;
}

unsigned int extract_brackets(std::string cmd, unsigned int index) {
	int flags = 1;
	for (; index < cmd.length(); index++) {
		if (cmd[index] == '(') flags++;
		else if (cmd[index] == ')') flags--;
		if (!flags) return index;
	}
	return 0;
}

bool syntax_check(std::string cmd) {
	// 检测原子表达式是否有空格
	if (cmd[0] != '(' && cmd.find_first_of(' ') != -1) {
		std::cout << "Atomic expressions are not allowed to have spaces" << std::endl;
		return false;
	}
	// 检测列表语法是否有错
	int flags = 0;
	int haveSpace = 0;
	for (unsigned int i = 0; i < cmd.length(); i++) {  
		if (cmd[i] == '(') flags++;
		if (cmd[i] == ')') {
			flags--;
			if (flags < 0) {
				std::cout << "Bracket mismatch" << std::endl;
				return false;
			}
		}
		if (cmd[i] == ' ') haveSpace = 1;
	}
	if (flags) {
		std::cout << "Bracket mismatch" << std::endl;
		return false;
	}
	if (!haveSpace && cmd[0] == '(') {
		std::cout << "Too few parameters" << std::endl;
		return false;
	}
	return true;
}

std::string interpret(std::string cmd) {  
	std::string result;
	if (cmd[0] != '(') {   // 原子
		return atom(cmd);
	}

	std::string identifier;
	unsigned i;
	for (i = 1; i < cmd.length(); i++) {
		if (cmd[i] == ' ' || cmd[i] == '(' || cmd[i] == ')') break;
		identifier += cmd[i];
	}

	// 处理 define，define 不能嵌套，所以如果有一定是第一个标识符
	if (identifier == "define") {
		if (define(cmd.substr(i + 1, cmd.length() - i - 2))) return "define";
		return "error";
	}

	// 处理嵌套函数
	int index = -1;
	if ((index = cmd.find("lambda")) != -1 && cmd[index-1] == '(' && cmd[index+6] == ' ') {
		std::cout << "find lambda" << std::endl;
	}

	// 处理 cond 函数
	while ((index = cmd.find("cond")) != -1 && cmd[index - 1] == '(' && (cmd[index + 4] == ' ')) {
		unsigned int end = extract_brackets(cmd, index);
		result = cond(cmd.substr(index+5, end-index-5));
		if (result == "error") return "error";
		cmd.replace(index-1, end - index+2, result);	
	}
	return general_expression(cmd);
}

std::string general_expression(std::string cmd) {
	// 处理常规表达式
	int length = 0;
	int index;
	std::string result;
	while ((index = cmd.find_last_of('(')) != -1) {       // 如果有括号说明还没有处理完
		for (length = 2; cmd[index + length - 1] != ')'; length++);    // 查找最小括号
		result = run(cmd.substr(index + 1, length - 2));  // 分割表达式
		if (result == "error") return "error";
		cmd.replace(index, length, result);  // 替代最小括号
	}
	return cmd;
}

bool define(std::string ide_var) {            // 处理定义表达式
	std::string identifier;               // 找到括号内的第一个单词，也就是操作符
	unsigned int i;
	for (i = 0; i < ide_var.length(); i++) {
		if (ide_var[i] == ' ') break;
		if (ide_var[i] == '(' || ide_var[i] == ')') {
			std::cout << "Error in defining parameters" << std::endl;
			return false;
		}
		identifier += ide_var[i];
	}
	// 如果操作符不在 map 里面，说明是不能识别的操作符
	if (identifier_var.find(identifier) != identifier_var.end()) {
		std::cout << "Variable are already bound" << std::endl;
		return false;
	}

	// 将 define 的内容提取出来，放到 map 里面
	std::string explain = ide_var.substr(i + 1, ide_var.length() - 1);
	if (explain == "True" || explain == "False") {  
		current_type = "Boolean";
	}
	else if (explain[0] >= '0') current_type = "Int";
	else current_type = "Fun";

	identifier_var[identifier] = var(0, current_type, explain);
	return true;
}

std::string run(std::string mini_formula) {  // 计算
	std::string identifier;
	unsigned i;
	for (i = 0; i < mini_formula.length(); i++) {
		if (mini_formula[i] == ' ') {
			i++;
			break;
		}
		identifier += mini_formula[i];
	}

	if (identifier_var.find(identifier) == identifier_var.end()) return "error";
	switch (identifier_var[identifier].ope)
	{
	case 1:                // 加法操作
	{
		std::string x = "";
		std::string y = "";
		while (mini_formula[i] != ' ') x += mini_formula[i++];
		i++;
		while (i < mini_formula.length()) y += mini_formula[i++];
		return std::to_string(std::stoi(x) + std::stoi(y));
	}
	case 2:       // 减法操作
	{
		std::string x = "";
		std::string y = "";
		while (mini_formula[i] != ' ') x += mini_formula[i++];
		i++;
		while (i < mini_formula.length()) y += mini_formula[i++];
		return std::to_string(std::stoi(x) - std::stoi(y));
	}
	case 3:  // 乘法操作
	{
		std::string x = "";
		std::string y = "";
		while (mini_formula[i] != ' ') x += mini_formula[i++];
		i++;
		while (i < mini_formula.length()) y += mini_formula[i++];
		return std::to_string(std::stoi(x) * std::stoi(y));
	}
	case 4:  // 除法操作
	{
		std::string x = "";
		std::string y = "";
		while (mini_formula[i] != ' ') x += mini_formula[i++];
		i++;
		while (i < mini_formula.length()) y += mini_formula[i++];
		return std::to_string(std::stoi(x) / std::stoi(y));
	}
	case 5:  // eq? 操作
	{
		std::string x = "";
		std::string y = "";
		while (mini_formula[i] != ' ') x += mini_formula[i++];
		i++;
		while (i < mini_formula.length()) y += mini_formula[i++];
		if (x == y) return "True";
		return "False";
	}
	case 6:
	default:
		break;
	}
}

std::string atom(std::string cmd) {  // 没有括号的表达式，例如 a
	if (identifier_var.find(cmd) != identifier_var.end()) {
		current_type = identifier_var[cmd].type;
		return identifier_var[cmd].explain;
	}
	else {
		std::cout << "Undefine : " << cmd << std::endl;
		return "error";
	}
}

std::string cond(std::string cmd) {
	// 开始处理 cond 函数 , 测试样例 (cond ((eq? 2 2) 2) (True 2))
	std::string result;
	while (cmd.length() > 0) {
		unsigned int end = extract_brackets(cmd, 1); // 提取第一个表达式
		result = handle_cond(cmd.substr(1, end-1));	
		if (result != "False") return result;
		cmd.replace(0, end + 1, "");
	}
	return "False";
}

std::string handle_cond(std::string cmd) {
	std::cout << cmd << std::endl;
	std::string result;
	if (cmd.substr(0, 4) == "True") {
		result = cmd.substr(5, cmd.length() - 5);
		return general_expression(result);
	}
	if (cmd[0] == '(') {
		unsigned end = extract_brackets(cmd, 1);
		if (general_expression(cmd.substr(0, end + 1)) == "True") {
			return general_expression(cmd.substr(end + 2, cmd.length() - end - 2));
		}
	}
	return "False";
}

int main() {
	std::string cmd;      
	init();
	while (1) {
		current_type = "";
		std::cout << ">>> ";
		std::getline(std::cin, cmd);              // 获取命令行
		if (cmd == "exit") break;                 // 退出指令  
		if (cmd.length() == 0) continue;          // 空指令
		cmd = format(cmd);                        // 格式化字符串
		if (cmd.length() == 0) {                  // 空格指令
			std::cout << "..." << std::endl;
			continue;
		}
		if (!syntax_check(cmd)) continue;         // 语法检测

		/******** 语义分析 ******/
		std::string result = interpret(cmd);
		if (result != "error") {
			std::cout << result << std::endl;
			if (result[0] >= '0' && result[0] <= '9') current_type = "Int";
			else if (result == "True" || result == "False") current_type = "Boolean";
			std::cout << current_type << std::endl;
		}
	}
	return 0;
}