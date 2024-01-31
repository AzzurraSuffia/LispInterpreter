#include "Parser.h"
#include "Token.h"
#include "Exceptions.h"
#include "NumExpr.h"
#include <unordered_map>
#include <sstream>

Statement* Parse::ParseStatement(std::vector<Token>::const_iterator& itr) {
		if (itr->tag == Token::SET) {
			//sposto l'iteratore sulla variabile
			safe_next(itr);
			if (itr->tag == Token::ID) {
				Variable* var = dynamic_cast<Variable*>(ParseNumExpr(itr));
				safe_next(itr);
				NumExpr* value = ParseNumExpr(itr);
				//Controllo che l'espressione termini con una parentesi chiusa
				safe_next(itr);
				if (itr->tag != Token::RP) {
					std::stringstream tmp{};
					tmp << "missing right parenthesis at token '" << itr->tag << ";" << itr->word << "'";
					throw ParseError(tmp.str());
					return nullptr;
				}
				return dynamic_cast<Statement*>(em.makeSetStmt(var, value));
			}
			else {
				std::stringstream tmp{};
				tmp << "expected a variable, got '" << itr->tag << ";" << itr->word << "'";
				throw ParseError(tmp.str());
				return nullptr;
			}

		}
		else if (itr->tag == Token::PRINT) {
				NumExpr* output;
				safe_next(itr);
				output = ParseNumExpr(itr);
				safe_next(itr);
				if (itr->tag != Token::RP) {
					std::stringstream tmp{};
					tmp << "missing right parenthesis at token '" << itr->tag << ";" << itr->word << "'";
					throw ParseError(tmp.str());
					return nullptr;
				}			
			return dynamic_cast<Statement*>(em.makePrintStmt(output));

		}
		else if (itr->tag == Token::INPUT) {
			//sposto l'iteratore sulla variabile
			safe_next(itr);
			if (itr->tag == Token::ID) {
				Variable* var = dynamic_cast<Variable*>(Parse::ParseNumExpr(itr));
				//Controllo che l'espressione termini con una parentesi chiusa
				safe_next(itr);
				if (itr->tag != Token::RP) {
					std::stringstream tmp{};
					tmp << "missing right parenthesis at token '" << itr->tag << ";" << itr->word << "'";
					throw ParseError(tmp.str());
					return nullptr;
				}
				return dynamic_cast<Statement*>(em.makeInputStmt(var));
			}
			else {
				std::stringstream tmp{};
				tmp << "expected a variable, got '" << itr->tag << ";" << itr->word << "'";
				throw ParseError(tmp.str());
				return nullptr;
			}

		}
		else if (itr->tag == Token::IF) {
			BoolExpr* condition = Parse::ParseBoolExpr(itr);
			//mi ritrovo l'iteratore sul token prima
			safe_next(itr);
			Block* t = Parse::ParseBlock(itr);
			//mi ritrovo l'iteratore sul token prima
			safe_next(itr);
			Block* f = Parse::ParseBlock(itr);
			//mi ritrovo l'iteratore sul token prima
			safe_next(itr);
			if (itr->tag != Token::RP) {
				std::stringstream tmp{};
				tmp << "missing right parenthesis at token '" << itr->tag << ";" << itr->word << "'";
				throw ParseError(tmp.str());
				return nullptr;
			}
			return dynamic_cast<Statement*>(em.makeIfStmt(condition, t, f));

		}
		else if (itr->tag == Token::WHILE) {
			BoolExpr* condition = Parse::ParseBoolExpr(itr);
			//mi ritrovo l'iteratore sul token prima
			safe_next(itr);
			Block* l = Parse::ParseBlock(itr);
			//mi ritrovo l'iteratore sul token prima
			safe_next(itr);
			if (itr->tag != Token::RP) {
				std::stringstream tmp{};
				tmp << "missing right parenthesis at token '" << itr->tag << ";" << itr->word << "'";
				throw ParseError(tmp.str());
				return nullptr;
			}
			return dynamic_cast<Statement*>(em.makeWhileStmt(condition, l));

		} else {
			throw ParseError("Unexpected Token");
			return nullptr;
		}
	
}

Program* Parse::ParseProgram(std::vector<Token>::const_iterator& itr) {
	Block* list = ParseBlock(itr);
	return dynamic_cast<Program*>(em.makeProgram(list));
}

Block* Parse::ParseBlock(std::vector<Token>::const_iterator& itr) {

	//un programma corretto inizia:
	//- per una parentesi e una parola chiave di uno statement
	//- per una parentesi e la parola chiave BLOCK

	if (itr->tag == Token::LP) {
		safe_next(itr);
		std::vector<Statement*> list;

		//c'è un solo statement
		if (itr->tag != Token::BLOCK) {
			Statement* s = ParseStatement(itr);
			list.push_back(s);
			return dynamic_cast<Block*>(em.makeBlock(list, false));
		}

		//se ho più di uno statement è necessaria la parola chiave BLOCK
		else {
			//sposto l'iteratore dopo BLOCK
			safe_next(itr);
			
			int count = 0;
			while (itr->tag == Token::LP) {
				safe_next(itr);
				Statement* s = ParseStatement(itr);
				list.push_back(s);
				safe_next(itr);
				count++;
			} 
			
			//controllo che ci sia almeno uno statement
			if (count < 1) {
				throw ParseError("empty BLOCK statement");
				return nullptr;
			}

			//controllo che ci sia la parentesi chiusa
			if (itr->tag == Token::RP) {
				return dynamic_cast<Block*>(em.makeBlock(list, true));
			}

			else {
				std::stringstream tmp{};
				tmp << "missing right parenthesis at token '" << itr->tag << ";" << itr->word << "'";
				throw ParseError(tmp.str());
				return nullptr;
			}

		}

	}
	else {
		//controllo che ci sia una parentesi all'inizio di un stmt_block
		std::stringstream tmp{};
		tmp << "missing left parenthesis at token '" << itr->tag << ";" << itr->word << "'";
		throw ParseError(tmp.str());
		return nullptr;
	}

}
