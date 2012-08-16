//retarget/retarget
#ifndef _RETARGET_RETARGET_HPP_
#define _RETARGET_RETARGET_HPP_




struct RetargetImpl{
    virtual ~RetargetImpl()=0;

    virtual int fputc(int ch)=0;
    virtual int fflush()=0;

    virtual int fgetc()=0;
    virtual void backspace()=0;
};




#endif /* _RETARGET_RETARGET_HPP_ */
