/*
 * nonblocking_queue.h
 *
 *  Created on: 4. 2. 2017
 *      Author: apollo
 */

#ifndef NON_BLOCKING_QUEUE_H_
#define NON_BLOCKING_QUEUE_H_

#include <mutex>
#include <deque>

// Neblokujici fronta

template < class T >
class safe_queue_2
{
	std::deque<T> m_queue;
	std::mutex m_lock;
public:
	void send( const T& val )
	{
		{
			std::lock_guard<std::mutex> hold(m_lock);
			m_queue.push_front( val );
		}
	}

	bool receive( T& val )
	{
		std::unique_lock<std::mutex> hold(m_lock);
		if (m_queue.size() == 0)
		{
			return false;
		}
		val = move( m_queue.back());
		m_queue.pop_back();
		return true;
	}
};

#endif /* NON_BLOCKING_QUEUE_H_ */
