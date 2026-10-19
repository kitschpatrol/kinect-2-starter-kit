#include "ofMain.h"

// Extract some stuff from Kinect.h...
// Todo figure out a better way to do this without too much dependency bulk

#ifndef _HandState_
#define _HandState_
typedef enum _HandState HandState;


enum _HandState
{
	HandState_Unknown = 0,
	HandState_NotTracked = 1,
	HandState_Open = 2,
	HandState_Closed = 3,
	HandState_Lasso = 4
};
#endif // _HandState_

#ifndef _JointType_
#define _JointType_
typedef enum _JointType JointType;


enum _JointType
{
	JointType_SpineBase = 0,
	JointType_SpineMid = 1,
	JointType_Neck = 2,
	JointType_Head = 3,
	JointType_ShoulderLeft = 4,
	JointType_ElbowLeft = 5,
	JointType_WristLeft = 6,
	JointType_HandLeft = 7,
	JointType_ShoulderRight = 8,
	JointType_ElbowRight = 9,
	JointType_WristRight = 10,
	JointType_HandRight = 11,
	JointType_HipLeft = 12,
	JointType_KneeLeft = 13,
	JointType_AnkleLeft = 14,
	JointType_FootLeft = 15,
	JointType_HipRight = 16,
	JointType_KneeRight = 17,
	JointType_AnkleRight = 18,
	JointType_FootRight = 19,
	JointType_SpineShoulder = 20,
	JointType_HandTipLeft = 21,
	JointType_ThumbLeft = 22,
	JointType_HandTipRight = 23,
	JointType_ThumbRight = 24,
	JointType_Count = (JointType_ThumbRight + 1)
};
#endif // _JointType_
#ifndef _TrackingState_
#define _TrackingState_
typedef enum _TrackingState TrackingState;


enum _TrackingState
{
	TrackingState_NotTracked = 0,
	TrackingState_Inferred = 1,
	TrackingState_Tracked = 2
};
#endif // _TrackingState_


#ifndef _Vector4_
#define _Vector4_
typedef struct _Vector4
{
	float x;
	float y;
	float z;
	float w;
} 	Vector4;

#endif // _Vector4_
#ifndef _PointF_
#define _PointF_
typedef struct _PointF
{
	float X;
	float Y;
} 	PointF;

#endif // _PointF_
#ifndef _ColorSpacePoint_
#define _ColorSpacePoint_
typedef struct _ColorSpacePoint
{
	float X;
	float Y;
} 	ColorSpacePoint;

#endif // _ColorSpacePoint_
#ifndef _DepthSpacePoint_
#define _DepthSpacePoint_
typedef struct _DepthSpacePoint
{
	float X;
	float Y;
} 	DepthSpacePoint;

#endif // _DepthSpacePoint_
#ifndef _CameraSpacePoint_
#define _CameraSpacePoint_
typedef struct _CameraSpacePoint
{
	float X;
	float Y;
	float Z;
} 	CameraSpacePoint;

#endif // _CameraSpacePoint_
#ifndef _Joint_
#define _Joint_
typedef struct _Joint
{
	JointType JointType;
	CameraSpacePoint Position;
	TrackingState TrackingState;
} 	Joint;

#endif // _Joint_
#ifndef _JointOrientation_
#define _JointOrientation_
typedef struct _JointOrientation
{
	JointType JointType;
	Vector4 Orientation;
} 	JointOrientation;

#endif // _JointOrientation_


class Kv2Joint
{
public:
	Kv2Joint() : trackingState(TrackingState_NotTracked) {}
	Kv2Joint(const _Joint& kcbPosition, const _JointOrientation& kcbOrientation)
	{
		jointOrientation.set(kcbOrientation.Orientation.x, kcbOrientation.Orientation.y, kcbOrientation.Orientation.z, kcbOrientation.Orientation.w);
		jointPosition.set(kcbPosition.Position.X, kcbPosition.Position.Y, kcbPosition.Position.Z);
		type = kcbPosition.JointType;
		trackingState = kcbPosition.TrackingState;
	}

	ofVec3f getPosition()
	{
		return jointPosition;
	}

	ofQuaternion getOrientation()
	{
		return jointOrientation;
	}

	TrackingState getTrackingState()
	{
		return trackingState;
	}

protected:
	ofVec3f jointPosition;
	ofQuaternion jointOrientation;
	JointType type;
	TrackingState trackingState;
};

class Kv2Skeleton
{
public:
	bool tracked;
	HandState leftHandState;
	HandState rightHandState;
	map<JointType, Kv2Joint> joints;
};