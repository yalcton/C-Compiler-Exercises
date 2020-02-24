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


        // TODO : handle the others:

    }
    else if(program->type=="Assign")
    { //Evaluate expression V, and assign it to variable N. The return value is the value of V.
      std::string zero = makeName("zero");
      std::cout << "const " << zero << " 0" << std::endl;

      // Assign the value of the assign branch to destReg
      CompileRec(destReg, program->branches.at(0));
      // Put the value in destReg into the id name
      std::cout << "add " << program->value << " " << destReg << " " << zero << std::endl;

    }
    else if(program->type=="Add")
    { //Evaluate A then B; return A + B.
      std::string left = makeName("left");
      std::string right = makeName("right");
      // Compile the left
      CompileRec(left, program->branches.at(0));
      // Compile the right
      CompileRec(right, program->branches.at(1));
      std::cout << "add " << destReg << " " << left << " " << right << std::endl;
    }
    else if(program->type=="Output")
    {  //Evaluate X, then send the result to output. The return value is the value of X.
      std::cout<<"output " <<(program->branches.at(0))->type<<std::endl;

    }
    else if(program->type=="Input")
    {   //Reads an integer from the input stream of numbers and returns it. Note that it is not a variable because it is not lower-case.
     std::cout<<"input "<<destReg<<std::endl;

    }

    else if(program->type=="LessThan")
    {  //Evaluate A then B; return non-zero if A < B.
      std::string left = makeName("left");
      std::string right = makeName("right");
      // Compile the left
      CompileRec(left, program->branches.at(0));
      // Compile the right
      CompileRec(right, program->branches.at(1));

      std::cout << "lt " << destReg << " " << left << " " << right << std::endl;
    }

    else if(program->type=="Sub")
    { //Evaluate A then B; return A - B.
      std::string left = makeName("left");
      std::string right = makeName("right");
      // Compile the left
      CompileRec(left, program->branches.at(0));
      // Compile the right
      CompileRec(right, program->branches.at(1));

      std::cout << "sub " << destReg << " " << left << " " << right << std::endl;
    }

    else if(program->type=="If")
    {
      std::string zero = makeName("zero");
      std::cout << "const " << zero << " 0" << std::endl;

      std::string c = makeName("condition");
      std::string stat2 = makeName("stat2");
      std::string end = makeName("end");
      // Compile the condition into destination register
      CompileRec(c, program->branches.at(0));
      // If the condition is equal to 0, skip to the else statement
      std::cout << "beq " << c << " " << zero << " " << stat2 << std::endl;
      CompileRec(destReg, program->branches.at(1));
      // Goto the end after the if statement
      std::cout << "beq " << zero << " " << zero << " " << end << std::endl;
      std::cout << ":" << stat2 << std::endl;
      CompileRec(destReg, program->branches.at(2));

      std::cout << ":" << end << std::endl;
    }
    else if(program->type=="While")
    {
      std::string zero = makeName("zero");
      std::cout << "const " << zero << " 0" << std::endl;
      std::string start = makeName("start");
      std::string end = makeName("end");
      std::string c = makeName("case");
      // Get register value for the condition
      // Set a label at the beginning of the
      //  loop
      std::cout << ":" << start << std::endl;
      // Compile the condition into destination register c
      CompileRec(destReg, program->branches.at(0));

      // At the beginning of the loop, check whether the
      // value in the condition register is equal to zero
      // if it is, go to the end
      std::cout << "beq " << destReg << " " << zero << " " << end << std::endl;
      // else do the rest of the code
      CompileRec(c, program->branches.at(1));
      // jump to the top again
      std::cout << "beq " << zero << " " << zero << " " << start << std::endl;
      // the end label
      std::cout << ":" << end << std::endl;
    }else
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
