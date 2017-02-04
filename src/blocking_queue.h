/*
 * blocking_queue.h
 *
 *  Created on: 4. 2. 2017
 *      Author: apollo
 */

#ifndef BLOCKING_QUEUE_H_
#define BLOCKING_QUEUE_H_

#include <mutex>
#include <deque>
#include <condition_variable>

// Blokujici fronta

template < class T >
class safe_queue//concurent_queue
{
	std::deque<T> m_queue;
	std::condition_variable m_cv;
	std::mutex m_lock;
public:
	void send( const T& val )
	{
		{
			std::lock_guard<std::mutex> hold(m_lock);
			m_queue.push_front( val );
		}
		m_cv.notify_one();
	}

	void receive( T& val )
	{
		std::unique_lock<std::mutex> hold(m_lock);
		m_cv.wait( hold, [=] {return !m_queue.empty();});
		val = move( m_queue.back());
		m_queue.pop_back();
	}
};


#endif /* BLOCKING_QUEUE_H_ */
