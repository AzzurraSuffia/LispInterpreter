#include <sstream>
#include <vector>

#include "Token.h"
#include "Exceptions.h"
#include "NumExpr.h"
#include "BoolExpr.h"
#include "Parser.h"

NumExpr* Parse::ParseNumExpr(std::vector<Token>::const_iterator& itr) {
	//Un'espressione numerica corretta inizia per un numero, per una variabile o per una parentesi aperta
	//Un'espressione booleana corretta inizia per una costante booleana o per una parentesi aperta
	if (itr->tag == Token::LP) {
		//Devo analizzare un'espressione "composita" del tipo (@ E1 E2), dove @ = {ADD, SUB, MUL, DIV, LT, GT, EQ}
		//Avanzo a @
		safe_next(itr);
		//Analizzo @
		Operator::OpCode op;
		switch (itr->tag) {
		case Token::ADD: op = Operator::PLUS;  break;
		case Token::SUB: op = Operator::MINUS; break;
		case Token::MUL: op = Operator::TIMES; break;
		case Token::DIV: op = Operator::DIV;   break;
		default:
			std::stringstream tmp{};
			tmp << "unrecognized operator " << itr->word; 
			throw ParseError(tmp.str());
			return nullptr;
		}
		safe_next(itr);
		NumExpr* left = ParseNumExpr(itr);
		//Analizzo E2 e mi trovo l'iteratore sulla parentesi chiusa
		safe_next(itr);
		NumExpr* right = ParseNumExpr(itr);
		safe_next(itr);
		if (itr->tag != Token::RP) {
			std::stringstream tmp{};
			tmp << "missing right parenthesis at token '" << itr->tag << ";" << itr->word << "'";
			throw ParseError(tmp.str());
			return nullptr;
		}
		return dynamic_cast<NumExpr*>(em.makeOperator(op, left, right)); //metto nel vector di em l'operatore
	}
	else if (itr->tag == Token::NUM) {
		std::stringstream temp{};
		temp << itr->word;
		int64_t value;
		temp >> value;
		Node* expr = em.makeNumber(value);
		//Restituisco il puntatore al numero
		return dynamic_cast<NumExpr*>(expr);
	}
	else if (itr->tag == Token::ID) {
		std::stringstream temp{};
		temp << itr->word;
		std::string id;
		temp >> id;
		//Creo una variabile
		Node* expr = em.makeVariable(id); 
		return dynamic_cast<NumExpr*>(expr);
	}
	else {
		std::stringstream tmp{};
		tmp << "cannot parse expression at token " << itr->word;
		throw ParseError(tmp.str());
		return nullptr;
	}
}

BoolExpr* Parse::ParseBoolExpr(std::vector<Token>::const_iterator& itr) {
	safe_next(itr);
	//Un'espressione numerica corretta inizia per un numero, per una variabile o per una parentesi aperta
	//Un'espressione booleana corretta inizia per una costante booleana o per una parentesi aperta
	if (itr->tag == Token::LP) {
		//Devo analizzare un'espressione "composita" del tipo (@ E1 E2), dove @ = {ADD, SUB, MUL, DIV, LT, GT, EQ}
		//Avanzo a @
		//(Nel progetto originale è un metodo della classe ParseExpression)
		safe_next(itr);
		//Analizzo @
		if (itr->tag == Token::LT || itr->tag == Token::GT || itr->tag == Token::EQ) {
			RelOp::OpCode op;
			switch (itr->tag) {
			case Token::LT: op = RelOp::LT;  break;
			case Token::GT: op = RelOp::GT; break;
			case Token::EQ: op = RelOp::EQ; break;
			}
			
			safe_next(itr);
			NumExpr* left = ParseNumExpr(itr);
			//Analizzo E2 e mi trovo l'iteratore sulla parentesi chiusa
			safe_next(itr);
			NumExpr* right = ParseNumExpr(itr);
			safe_next(itr);
			if (itr->tag != Token::RP) {
				std::stringstream tmp{};
				tmp << "missing right parenthesis at token '" << itr->tag << ";" << itr->word << "'";
				throw ParseError(tmp.str());
				return nullptr;
			}
			return dynamic_cast<BoolExpr*>(em.makeRelOp(op, left, right));
		}
		else {
			BoolOp::OpCode op;
			switch (itr->tag) {
			case Token::AND: op = BoolOp::AND; break;
			case Token::OR:  op = BoolOp::OR;  break;
			case Token::NOT: op = BoolOp::NOT; break;
			default:
				std::stringstream tmp{};
				tmp << "unrecognized operator " << itr->word;
				throw ParseError(tmp.str());
				return nullptr;
			}
			//Avanzo a E1
			//Analizzo E1 e mi trovo l'iteratore su E2
			BoolExpr* left = ParseBoolExpr(itr);
			//NOT è un operatore unario, faccio sì che right e left puntino alla stessa espressione
			BoolExpr*& right = left;
			//AND e OR sono invece operatori binari
			if (op != BoolOp::NOT) {
				//Analizzo E2 e mi trovo l'iteratore sulla parentesi chiusa
				BoolExpr* right = ParseBoolExpr(itr);
			}
			//Controllo che ci sia una parentesi chiusa
			safe_next(itr);
			if (itr->tag != Token::RP) {
				std::stringstream tmp{};
				tmp << "missing right parenthesis at token '" << itr->tag << ";" << itr->word << "'";
				throw ParseError(tmp.str());
				return nullptr;
			}
			return dynamic_cast<BoolExpr*>(em.makeBoolOp(op, left, right));
		}
	}
	else if (itr->tag == Token::TRUE) {
		Node* expr = em.makeBoolConst(true);
		return dynamic_cast<BoolExpr*>(expr);
	}
	else if (itr->tag == Token::FALSE) {
		Node* expr = em.makeBoolConst(false);
		return dynamic_cast<BoolExpr*>(expr);
	}
	else {
		std::stringstream tmp{};
		tmp << "cannot parse expression at token " << itr->word;
		throw ParseError(tmp.str());
		return nullptr;
	}
}