add_executable( SimplePolygon SimplePolygon.cpp )
target_link_libraries( SimplePolygon ${OpenCV_LIBS} )

add_executable( Clipping Clipping.cpp )
target_link_libraries( Clipping ${OpenCV_LIBS} )

add_executable( PointPolygonTest PointPolygonTest.cpp )
target_link_libraries( PointPolygonTest ${OpenCV_LIBS} )

add_executable( ROI ROI.cpp )
target_link_libraries( ROI ${OpenCV_LIBS} )

add_executable( CVT CVT.cpp )
target_link_libraries( CVT ${OpenCV_LIBS} )