//retarget/retarget
#ifndef _RETARGET_RETARGET_HPP_
#define _RETARGET_RETARGET_HPP_




struct RetargetWriteImpl{
    virtual ~RetargetWriteImpl(){};

    virtual int fputc(int ch){
        return 0;
    }
    virtual int fflush(){
        return 0;
    }
};

struct RetargetReadImpl{
    virtual ~RetargetReadImpl(){};

    virtual int fgetc(){
        return 0;
    }
    virtual int backspace(){
        return 0;
    }
};




#endif /* _RETARGET_RETARGET_HPP_ */
