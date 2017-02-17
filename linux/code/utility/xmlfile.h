#ifndef XML_FILE_H
#define XML_FILE_H
/*	portable include  */
#include "typedefine.h"
#include "file.h"


class CXmlFile : public CStreamFile 
{
#define READ_BUFF_SIZE 	                256
#define ELEM_STACK_DEEP_MAX         10
#define VALUE_BUFFER_MAX            4096

    protected:
		u32             Depth;
		s8               CurValuebuf[VALUE_BUFFER_MAX];
		u32             CurValueLenth ;

	public:
		CXmlFile(const s8 *pFileName);
		~CXmlFile(void);

		void StartValue(const char *el, const char **attr);
		void EndValue(const char *el);
		void AddValue(s8 *s, int len);

		bool Load();
        
/*  user extend interface */        
            virtual void UserInitBeforeLoad(void){} ;
            virtual void UserStartValue(const char *el, const char **attr) {};
            virtual void UserEndValue(const char *el){} ;

	private:
		bool DeepReachMax()
		{
			return (Depth >= ELEM_STACK_DEEP_MAX);
		};

};

#endif
