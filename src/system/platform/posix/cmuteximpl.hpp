#ifndef EE_SYSTEMCMUTEXIMPLPOSIX_HPP
#define EE_SYSTEMCMUTEXIMPLPOSIX_HPP

#include "../../../declares.hpp"

#if defined( EE_PLATFORM_POSIX )

#include <pthread.h>

namespace EE { namespace System { namespace Platform {

class cMutexImpl {
	public:
		cMutexImpl();

		~cMutexImpl();

		void Lock();

		void Unlock();
	private:
		pthread_mutex_t mMutex;
};

}}}

#endif

#endif
 