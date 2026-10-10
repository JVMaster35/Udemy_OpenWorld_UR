#pragma once

#include "DrawDebugHelpers.h"

#define BLUE FColor::Blue

#define DRAW_SPHERE(location) if(GetWorld()) DrawDebugSphere(GetWorld(), location, 25.0f, 24.0f, FColor::Purple, true)
#define DRAW_SPHERE_SF(location) if(GetWorld()) DrawDebugSphere(GetWorld(), location, 25.0f, 24.0f, FColor::Purple, false, -1.0f, 0, 0.5f)

#define DRAW_LINE(start, end) if(GetWorld()) DrawDebugLine(GetWorld(), start, end, BLUE, true)
#define DRAW_LINE_SF(start, end) if(GetWorld()) DrawDebugLine(GetWorld(), start, end, BLUE, false, -1.0f, 0, 0.5f)

#define DRAW_POINT(location) if(GetWorld()) DrawDebugPoint(GetWorld(), location, 15.0f, FColor::Green, true)
#define DRAW_POINT_SF(location) if(GetWorld()) DrawDebugPoint(GetWorld(), location, 15.0f, FColor::Green, false, -1.0f, 0)

#define DRAW_BOX(location, extent) if(GetWorld()) DrawDebugBox(GetWorld(), location, extent, BLUE, true)
#define DRAW_BOX_SF(location, extent) if(GetWorld()) DrawDebugBox(GetWorld(), location, extent, BLUE, false, -1.0f, 0, 0.5f)

#define DRAW_VECTOR(start, end) if (GetWorld()) \
{\
DrawDebugLine(GetWorld(), start, end, BLUE, true);\
DrawDebugPoint(GetWorld(), end, 15.0f, FColor::Green, true);\
}
#define DRAW_VECTOR_SF(start, end) if (GetWorld()) \
{\
DrawDebugLine(GetWorld(), start, end, BLUE, false, -1.0f, 0, 0.5f);\
DrawDebugPoint(GetWorld(), end, 15.0f, FColor::Green, false, -1.0f, 0);\
}