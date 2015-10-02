
#ifndef __AREA_H__
#define __AREA_H__

#include "corefunc.h"

namespace oir {

	class CPlate {
	public:
		bool bColored;

		//! ���캯��
		CPlate(){
			bColored = true;
		}

		//! �������ȡ����
		inline void setPlateMat(Mat param) { m_plateMat = param; }
		inline Mat getPlateMat() const { return m_plateMat; }

		inline void setPlatePos(RotatedRect param) { m_platePos = param; }
		inline RotatedRect getPlatePos() const { return m_platePos; }

		inline void setPlateStr(String param) { m_plateStr = param; }
		inline String getPlateStr() const { return m_plateStr; }

		inline void setPlateLocateType(LocateType param) { m_locateType = param; }
		inline LocateType getPlateLocateType() const { return m_locateType; }

	private:
		//! ���Ƶ�ͼ��
		Mat m_plateMat;

		//! ������ԭͼ��λ��
		RotatedRect m_platePos;

		//! �����ַ���
		String m_plateStr;

		//! ���ƶ�λ�ķ���
		LocateType m_locateType;
	};

} /*! \namespace oir*/

#endif /* endif __AREA_H__ */