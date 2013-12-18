/*
 * FileName: TWGLGesturePlugin.h
 * Author: TimeLink Inc.
 * Modified: 2012-04-28
 * Email: sdk@timelink.cn
 * Comment: This defines the interface, data and helper for extending gesture library.
 */

#ifndef __TWGLGESTUREPLUGIN_H__
#define __TWGLGESTUREPLUGIN_H__ 

#include <sys/timeb.h>
#include <cmath>
#include <string>

/*
Our 2D-Coordinate use real number from 0.0 to 1.0, 
x-axis is increased from left to right, 
y-axis is increased from top to bottom. 

  (x,y)
(0.0,0.0) ---------------> (1.0,1.0)
    |
    |
    |
    |
    |
    |
    |
    V
(0.0,1.0)
*/

namespace touchwin { namespace sdk { namespace gesturelibrary {

	// represents a point of 2D-Coordinate for the screen.
	template<typename T>
	class Point
	{
	public:
		T x;
		T y;

	public:
		Point<T>(T x = static_cast<T>(0), T y = static_cast<T>(0))
			: x(x), y(y)
		{}
		bool operator != (const Point<T>& rhs) const
		{
			return (this->x != rhs.x || this->y != rhs.y);
		}
		bool operator == (const Point<T>& rhs) const
		{
			return (this->x == rhs.x && this->y == rhs.y);
		}
	};


	// class to represent the timestamp of contacts.
	class Timestamp
	{
	private:
		__int64 milliseconds;
	public:
		explicit
		Timestamp(__int64 millisecondsOffset = 0)
		{
			_timeb time;
			_ftime_s(&time);
			/*
			 * we do not care the overflow.
			 */
			milliseconds = time.time * 1000 + time.millitm + millisecondsOffset;
		}
		Timestamp(const Timestamp& other) : milliseconds(other.milliseconds)
		{
		}
		Timestamp& operator=(const Timestamp& rhs)
		{
			this->milliseconds = rhs.milliseconds;
			return *this;
		}
	public:
		Timestamp offset(__int64 millisecondsOffset) const
		{
			Timestamp newTimestamp(*this);
			newTimestamp.milliseconds += millisecondsOffset;
			return newTimestamp;
		}
	public:
		bool operator < (const Timestamp& rhs) const
		{
			return (this->milliseconds < rhs.milliseconds);
		}
		bool operator > (const Timestamp& rhs) const
		{
			return (this->milliseconds > rhs.milliseconds);
		}
	};


	// class to represent the touch-point on the screen.
	template<typename T>
	class Contact
	{
	public:
		enum Phase {
			Invalid,
			Down,
			Up,
			Move,
			Hold,
		};
	public:
		/* Each touch point has a numeric identifier, 
		   we guarantee that this id would not be reused at the same time. */
		size_t id;
		/* The center of touch point at the moment. */
		Point<T> location;
		/* The center of touch point at the moment this point down. */
		Point<T> originalLocation;
		/* Touch point - far from a point - has its width and height. */
		T width;
		T height;
		/* The delta-x and delta-y from this touch point last update. */
		T dx;
		T dy;
		/* A timestamp at the moment this point down. */
		Timestamp beginTime;
		/* Current status of this touch point. */
		Phase status;
		bool frameEnd;

	public:
		Contact<T>()
			: id(0)
			, location()
			, originalLocation()
			, width(0)
			, height(0)
			, dx(0)
			, dy(0)
			, beginTime()
			, status(Invalid)
			, frameEnd(false)
		{};
		Contact<T>(
			size_t id, 
			T width = static_cast<T>(0), 
			T height = static_cast<T>(0), 
			T x = static_cast<T>(0), 
			T y = static_cast<T>(0), 
			T dx = static_cast<T>(0), 
			T dy = static_cast<T>(0))
			: id(id)
			, location(x, y)
			, originalLocation(x ,y)
			, width(width)
			, height(height)
			, dx(dx)
			, dy(dy)
			, beginTime()
			, status(Down)
			, frameEnd(false)
		{};
		Contact<T>( const Contact<T>& other )
			: id(other.id)
			, location(other.location)
			, originalLocation(other.originalLocation)
			, width(other.width)
			, height(other.height)
			, dx(other.dx)
			, dy(other.dy)
			, beginTime(other.beginTime)
			, status(other.status)
			, frameEnd(other.frameEnd)
		{};
	};

	class ScreenRuler
	{
	private:
		float _width;
		float _height;
	public:
		ScreenRuler(float width = 475.97f, float height = 267.73f)
			: _width(width), _height(height) {};
		ScreenRuler(float diagonal, size_t aspectRatioX, size_t aspectRatioY)
			: _width(0.0f), _height(0.0f)
		{
			float aspectRatioDiagonal = std::sqrt(
				static_cast<float>(aspectRatioX * aspectRatioX + aspectRatioY * aspectRatioY));
			float baseLength = diagonal * 25.4f / aspectRatioDiagonal;
			_width = baseLength * aspectRatioX;
			_height = baseLength * aspectRatioY;
		};
	public:
		template <typename T>
		T gageWidth(T relativeWidth)
		{
			return relativeWidth * _width;
		};
		template <typename T>
		T gageHeight(T relativeHeight)
		{
			return relativeHeight * _height;
		};
		template <typename T>
		T computeDistance(const Point<T>& a, const Point<T>& b)
		{
			T deltaX = (b.x - a.x) * _width;
			T deltaY = (b.y - a.y) * _height;
			return std::sqrt(deltaX * deltaX + deltaY * deltaY);
		};
		template <typename T>
		T computeAngleOfSlope(const Point<T>& a, const Point<T>& b)
		{
			T deltaX = b.x - a.x;
			T deltaY = b.y - a.y;

			if (std::abs(deltaX - 0.0f) < 0.001f) /* X ~~ 0 */
			{
				return (deltaY < 0.0f) ? static_cast<T>(270) : static_cast<T>(90);
			} 
			else if (std::abs(deltaY - 0.0f) < 0.001f)  /* Y ~~ 0 */
			{
				return (deltaX < 0.0f) ? static_cast<T>(180) : static_cast<T>(0);
			}
			else
			{
				float theta = atan(
					std::abs(static_cast<float>(deltaY) / deltaX * _height / _width)) 
					* 180.0f / 3.1415926f;
				return 
					(deltaY > 0.0) 
					? (deltaX > 0.0) 
					? static_cast<T>(theta) 
					: static_cast<T>(180.0f - theta)
					: (deltaX > 0.0) 
					? static_cast<T>(360.0f - theta) 
					: static_cast<T>(180.0f + theta);
			}
		};
	};


	typedef Point<float> twPointf;
	typedef Contact<float> twTouchPointf;
	typedef Contact<float>::Phase twPointStatus;
	/* Indicates the point is push down and begin at the moment. */
	#define POINT_DOWN Contact<float>::Down
	/* Indicates the point is lift up and end at the moment. */
	#define POINT_UP Contact<float>::Up
	/* Indicates the point has move from last update. */
	#define POINT_MOVE Contact<float>::Move
	/* Nothing. */
	#define POINT_HOLD Contact<float>::Hold
	#define TWGL touchwin::sdk::gesturelibrary


	class twIGesture
	{
	public:
		virtual int updatePoint(const twTouchPointf& point, std::string& event) = 0; 
		virtual void setAttribute(const std::string& name, const std::string& value) = 0;
		virtual void release(void) = 0;
	};


	class twIGestureFactory
	{
	public:
		virtual bool hasGesture(const std::string& name) = 0;
		virtual twIGesture* getGesutre(const std::string& name) = 0;
	};
}}};
#endif
