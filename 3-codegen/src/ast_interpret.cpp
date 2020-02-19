#include "ast.hpp"

#include <regex>

int32_t Interpret(
    InterpretContext &context, // Contains the parameters and variable bindings
    TreePtr program
){
    std::regex reNum("^-?[0-9]+$");
    std::regex reId("^[a-z][a-z0-9]*$");

    if( regex_match(program->type, reNum) ){
        return std::atol(program->type.c_str());
    }
    // TODO : Check for things matching reId
    else if( regex_match(program->type, reId) ){ // TODO : Check for things matching reId

        return (context.bindings[program->type]);

    }else if(program->type=="Param"){
        unsigned index=atol(program->value.c_str());
        auto value=context.params.at(index);
        return value;

    }else if(program->type=="Output"){
        int32_t val=Interpret(context, program->branches.at(0));
        std::cout<<val<<std::endl;
        return val;

    // TODO: Handle other constructs
  }else if(program->type=="Seq"){
        int32_t A;
        for(int i=0; i<program->branches.size(); i++){
              A = Interpret(context, program->branches[i]);
        }
        return A;

  }else if(program->type=="Assign"){
    std::string N = program -> value;
    int32_t Temp = Interpret(context, program->branches.at(0));
    context.bindings[N] = Temp;
    return Temp;

  }else if(program->type=="Input"){
      int32_t val;
      std::cin >> val;
      return val;
  }else if(program->type=="LessThan"){
      int32_t A = Interpret(context,program->branches.at(0));
      int32_t B = Interpret(context,program->branches.at(1));
      if(A < B){
        return 1;
      }else{
        return 0;
      }
  }else if(program->type=="Add"){
      int32_t A = Interpret(context,program->branches.at(0));
      int32_t B = Interpret(context,program->branches.at(1));
      return A+B;
  }else if(program->type=="Sub"){
      int32_t A = Interpret(context,program->branches.at(0));
      int32_t B = Interpret(context,program->branches.at(1));
      return A-B;
  }else if(program->type=="If"){
          int32_t A = Interpret(context,program->branches.at(0));
          if (A != 0){
            int32_t B = Interpret(context,program->branches.at(1));
            return B;
          }else{
            int32_t C = Interpret(context,program->branches.at(2));
            return C;
          }
  }else if(program->type=="While"){
          while (Interpret(context, program->branches.at(0)) != 0){
            A = Interpret(context,program->branches.at(1));
          }
          return 0;




  }else{
      throw std::runtime_error("Unknown construct '"+program->type+"'");
    }
}
