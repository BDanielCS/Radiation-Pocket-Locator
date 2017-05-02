========================================================================
    CONSOLE APPLICATION : Radiaton Pocket Locator Project Overview
                            @author = Brandon Daniel
========================================================================

Radiation Graph as Designed During the MLH Prime Hackathon

 =======================================================================
                            RADIATION GRAPH
 =======================================================================
 
    Aims to create a versatile and efficent graph-based API in order to perform
analysis on radiation pockets in 3D space.  Standard graph operations are 
implemented with the exception of the periodic sweeper.   Parallel cleaning 
of removed nodes happens in systematic intervals in order to minimize the 
amount of time during traversal and maximize performance in terms of insertions.

    Implemted clustering feature in order to focus on more densely/strongly
connected areas of the graph.  Feature is aimed towards use during analysis.

=========================================================================
                                Analysis
=========================================================================

Post agregation of data, statistic analysis can be performed on any of the
sectors of 3D space.  Confidence intervals around all desired estimators 
can be drawn to make assumptions about the distribution of radiation on
a given surface.

