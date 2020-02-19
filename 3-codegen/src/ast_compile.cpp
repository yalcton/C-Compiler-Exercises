#include "ast.hpp"

#include <string>
#include <regex>

static int makeNameUnq=0;

static std::string makeName(std::string base)
{
    return "_"+base+"_"+std::to_string(makeNameUnq++);
}

void CompileRec(
    std::string destReg,    // The name of the register to put the result in
    TreePtr program
){
    std::regex reNum("^-?[0-9]+$");
    std::regex reId("^[a-z][a-z0-9]*$");

    if( regex_match( program->type, reNum ) ){
        std::cout<<"const "<<destReg<<" "<<program->type<<std::endl;

    }else if( regex_match( program->type, reId ) ){
        std::string zero = makeName("zero");
        std::cout<<"const "<<zero<<" 0"<<std::endl;
        std::cout<<"add "<<destReg<<" "<<program->type<<" "<<zero<<std::endl;

    }else if(program->type=="Param"){
        std::cout<<"param "<<destReg<<" "<<program->value<<std::endl;

    }else if(program->type=="Seq"){   ///Executes X, then Y, and so on. Return value is that of the last part executed. There is always at least one operand, i.e. Seq [ ] is not a valid construct.
        for(unsigned i=0; i<program->branches.size(); i++){
            CompileRec(destReg, program->branches[i]);
        }
        // TODO : handle the others, doing in order on spec

    }
    else if(program->type=="Input")
    {   //Reads an integer from the input stream of numbers and returns it. Note that it is not a variable because it is not lower-case.
     std::cout<<"input "<<destReg<<std::endl;

    }
    else if(program->type=="Output")
    {  //Evaluate X, then send the result to output. The return value is the value of X.
      std::cout<<"output " <<(program->branches.at(0))->type<<std::endl;

    }
    else if(program->type=="LessThan")
    {  //Evaluate A then B; return non-zero if A < B.
      if( regex_match( (program->branches.at(0))->type, reNum))
      {
        std::cout<<"const "<<(program->branches.at(0))->type<<" "<<(program->branches.at(0))->type<<std::endl;
      }
      else if( regex_match( (program->branches.at(1))->type, reNum ))
      {
        std::cout<<"const "<<(program->branches.at(1))->type<<" "<<(program->branches.at(1))->type<<std::endl;
      }
      std::cout<<"less than "<<destReg<<" "<<(program->branches.at(0))->type<<" "<<(program->branches.at(1))->type<<std::endl;

    }
    else if(program->type=="Add")
    { //Evaluate A then B; return A + B.
      if(regex_match((program->branches.at(0))->type,reNum))
      {
        std::cout<<"const "<<(program->branches.at(0))->type<<" "<<(program->branches.at(0))->type<<std::endl;
      }
      else if( regex_match( (program->branches.at(1))->type, reNum ))
      {
        std::cout<<"const "<<(program->branches.at(1))->type<<" "<<(program->branches.at(1))->type<<std::endl;
      }
        std::cout<<"add "<<destReg<<" "<<(program->branches.at(0))->type<<" "<<(program->branches.at(1))->type<<std::endl;
    }
    else if(program->type=="Sub")
    { //Evaluate A then B; return A - B.
    if( regex_match( (program->branches.at(0))->type, reNum))
    {
      std::cout<<"const "<<(program->branches.at(0))->type<<" "<<(program->branches.at(0))->type<<std::endl;
    }
    else if( regex_match( (program->branches.at(1))->type, reNum ))
    {
      std::cout<<"const "<<(program->branches.at(1))->type<<" "<<(program->branches.at(1))->type<<std::endl;
    }
    std::cout<<"sub "<<destReg<<" "<<(program->branches.at(0))->type<<" "<<(program->branches.at(1))->type<<std::endl;
    }
    else if(program->type=="Assign")
    { //Evaluate expression V, and assign it to variable N. The return value is the value of V.
    CompileRec(program->value,program->branches.at(0)); //CompileRec will call itself to assign the new values.

    }
    else if(program->type=="If")
    {
      std::string jumped = makeName("jump");
      std::string ifthenelse = makeName("else");
      std::string zero = makeName("zero");
      std::cout << "const " << zero << " 0"<< std::endl;
      CompileRec((program->branches.at(0))->type,program->branches.at(0));
      std::cout << "branch if equal " << (program->branches.at(0))->type << " " << zero << " " << ifthenelse <<std::endl;
      CompileRec(destReg,program->branches.at(1));
      std::cout<<"branch if equal " << zero << " " << zero << " " << jumped <<std::endl;
      std::cout << ":" << ifthenelse << std::endl;
      CompileRec(destReg,program->branches.at(2));
      std::cout << ":" << jumped << std::endl;
    }
    // else if(program->type=="While")
    // {
    //   std::string labeltop = makeName("top");
    //   std::string labelbottom = makeName("bottom");
    //   std::string zero = makeName("zero");
    //   std::cout << ":" << labeltop << std::endl;
    //   std::cout << "const " << zero << " 0"<< std::endl;
    //   std::cout << "beq " << (program->branches.at(0))->type << " " << zero << " " << labelbottom <<std::endl;
    //   CompileRec((program->branches.at(0))->type,program->branches.at(1));
    //   std::cout<<"beq " << zero << " " << zero << " " << labeltop <<std::endl;
    //   std::cout<<":"<<labelbottom<<std::endl;
    //   CompileRec(destReg,program->branches.at(0));
    else
    {
        throw std::runtime_error("Unknown construct '"+program->type+"'");
    }
}

void Compile(
    TreePtr program
){
    // Create a register to hold the final result
    std::string res=makeName("res");

    // Compile the whole thing
    CompileRec(res, program);

    // Put the result out
    std::cout<<"halt "<<res<<"\n";
}
