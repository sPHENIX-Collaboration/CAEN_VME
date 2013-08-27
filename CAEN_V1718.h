#ifndef __CAEN_V1718_H__
#define __CAEN_V1718_H__

// this is the "handle" singleton, hich maintains
// a single handle to the V1718 board.

// at this point, we do not support more than one, but for future
// expandability, the handle call takes an index.  


class CAEN_V1718
{
 public:

  static int Handle( const int index=0);
  static int Finish( const int index=0);

 protected:
  CAEN_V1718();
  virtual ~CAEN_V1718();

  virtual int getHandle() const { return _handle;};
  virtual int VMEReset(const int index= 0);
  int _handle;

  static CAEN_V1718 *_instance;
};

#endif




