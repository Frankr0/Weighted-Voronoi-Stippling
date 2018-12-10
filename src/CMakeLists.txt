add_executable( Stippling Stippling.cpp )
target_link_libraries( Stippling ${OpenCV_LIBS} )

# add_executable( SimplePolygon SimplePolygon.cpp )
# target_link_libraries( SimplePolygon ${OpenCV_LIBS} )

add_executable( Utils Utils.cpp )
# target_link_libraries( Utils ${OpenCV_LIBS} )

add_executable( PointPolygonTest PointPolygonTest.cpp )
# target_link_libraries( PointPolygonTest ${OpenCV_LIBS} )
