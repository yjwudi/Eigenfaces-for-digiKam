/** ===========================================================
 * @file
 *
 * This file is a part of digiKam project
 * <a href="http://www.digikam.org">http://www.digikam.org</a>
 *
 * @date    2012-01-03
 * @brief   <a href="http://docs.opencv.org/modules/contrib/doc/facerec/facerec_tutorial.html#local-binary-patterns-histograms">Face Recognition based on Local Binary Patterns Histograms</a>
 *          Ahonen T, Hadid A. and Pietikäinen M. "Face description with local binary
 *          patterns: Application to face recognition." IEEE Transactions on Pattern
 *          Analysis and Machine Intelligence, 28(12):2037-2041.
 *
 * @section DESCRIPTION
 *
 * @author Copyright (C) 2012-2013 by Marcel Wiesweg
 *         <a href="mailto:marcel dot wiesweg at gmx dot de">marcel dot wiesweg at gmx dot de</a>
 * @author Copyright (c) 2011-2012 Philipp Wagner
 *         <a href="mailto:bytefish at gmx dot de">bytefish at gmx dot de</a>
 *
 * @section LICENSE
 *
 * Released to public domain under terms of the BSD Simplified license.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of the organization nor the names of its contributors
 *     may be used to endorse or promote products derived from this software
 *     without specific prior written permission.
 *
 *   See <http://www.opensource.org/licenses/bsd-license>
 *
 * ============================================================ */

//#include "libopencv.h"

//#if !OPENCV_TEST_VERSION(3,0,0)
#include "../EigenFaces/face.hpp"
//#endif

// C++ includes

//#include <vector>
#include "debugger.h"

namespace FacesEngine
{
/*
#if OPENCV_TEST_VERSION(3,0,0)
class LBPHFaceRecognizer : public cv::FaceRecognizer
#else
class LBPHFaceRecognizer : public cv::face::FaceRecognizer
#endif
*/
class FisherFaceRecognizer : public cv::face::FaceRecognizer
{
public:

    enum PredictionStatistics
    {
        NearestNeighbor,
        NearestMean,
        MostNearestNeighbors // makes only sense if there is a threshold!
    };

public:

    // Initializes this LBPH Model. The current implementation is rather fixed
    // as it uses the Extended Local Binary Patterns per default.
    //
    // radius, neighbors are used in the local binary patterns creation.
    // grid_x, grid_y control the grid size of the spatial histograms.
    FisherFaceRecognizer()
    {
        m_threshold = 25000;
    }

    // Initializes and computes this LBPH Model. The current implementation is
    // rather fixed as it uses the Extended Local Binary Patterns per default.
    //
    // (radius=1), (neighbors=8) are used in the local binary patterns creation.
    // (grid_x=8), (grid_y=8) controls the grid size of the spatial histograms.
    FisherFaceRecognizer(cv::InputArrayOfArrays src,
                       cv::InputArray labels)
    {
        m_threshold = 25000;
        train(src, labels);
    }

    ~FisherFaceRecognizer() {}

//#if OPENCV_TEST_VERSION(3,0,0)
    using cv::face::FaceRecognizer::save;
    using cv::face::FaceRecognizer::load;
/*
#else
    using cv::face::FaceRecognizer::save;
    using cv::face::FaceRecognizer::load;
#endif
*/

    static cv::Ptr<FisherFaceRecognizer> create();
    void train(cv::InputArrayOfArrays src, cv::InputArray labels);
    void update(cv::InputArrayOfArrays src, cv::InputArray labels);
    int predict(cv::InputArray src) const;
    void predict(cv::InputArray _src, int &label, double &dist) const;

    std::vector<cv::Mat> getSrc() const       { return m_src;          }
    void setSrc(std::vector<cv::Mat> new_src) { m_src = new_src;       }

    cv::Mat getLabels() const                 { return m_labels;       }
    void setLabels(cv::Mat new_labels)        { m_labels = new_labels; }

    void save(cv::FileStorage&) const         {}
    void load(const FileStorage& fs)          {}



/*

    /**
     * Computes a LBPH model with images in src and
     * corresponding labels in labels.
     /
#if OPENCV_TEST_VERSION(3,1,0)
    void train(cv::InputArrayOfArrays src, cv::InputArray labels);
#else
    void train(cv::InputArrayOfArrays src, cv::InputArray labels) override;
#endif

    /**
     * Updates this LBPH model with images in src and
     * corresponding labels in labels.
     /
#if OPENCV_TEST_VERSION(3,1,0)
    void update(cv::InputArrayOfArrays src, cv::InputArray labels);
#else
    void update(cv::InputArrayOfArrays src, cv::InputArray labels) override;
#endif


#if OPENCV_TEST_VERSION(3,1,0)
    /**
     * Predicts the label of a query image in src.
     /
    int predict(cv::InputArray src) const;

    /**
     * Predicts the label and confidence for a given sample.
     /
    void predict(cv::InputArray _src, int &label, double &dist) const;
#else
    using cv::face::FaceRecognizer::predict;
    /*
     * Predict
     /
    void predict(cv::InputArray src, cv::Ptr<cv::face::PredictCollector> collector) const override;
#endif

    /**
     * See FaceRecognizer::load().
     /
#if OPENCV_TEST_VERSION(3,1,0)
    void load(const cv::FileStorage&) {}
#else
    void load(const cv::FileStorage&) override {}
#endif

    /**
     * See FaceRecognizer::save().
     /
#if OPENCV_TEST_VERSION(3,1,0)
    void save(cv::FileStorage&) const {}
#else
    void save(cv::FileStorage&) const override {}
#endif

    /**
     * Getter functions.
     /
#if OPENCV_TEST_VERSION(3,0,0)

    int neighbors() const { return m_neighbors; }
    int radius()    const { return m_radius;    }
    int grid_x()    const { return m_grid_x;    }
    int grid_y()    const { return m_grid_y;    }

    // NOTE: Implementation done through CV_INIT_ALGORITHM macro from OpenCV.
    cv::AlgorithmInfo* info() const;

#else

    int getNeighbors() const                             { return m_neighbors;            }
    void setNeighbors(int _neighbors)                    { m_neighbors = _neighbors;      }

    int getRadius()    const                             { return m_radius;               }
    void setRadius(int radius)                           { m_radius = radius;             }

    int getGrid_x()    const                             { return m_grid_x;               }
    void setGrid_x(int _grid_x)                          { m_grid_x = _grid_x;            }

    int getGrid_y()    const                             { return m_grid_y;               }
    void setGrid_y(int _grid_y)                          { m_grid_y = _grid_y;            }

    double getThreshold() const override                 { return m_threshold;            }
    void setThreshold(double _threshold)                 { m_threshold = _threshold;      }

    void setHistograms(std::vector<cv::Mat> _histograms) { m_histograms = _histograms;    }
    std::vector<cv::Mat> getHistograms() const           { return m_histograms;           }

    void setLabels(cv::Mat _labels)                      { m_labels = _labels;            }
    cv::Mat getLabels() const                            { return m_labels;               }

    void setStatistic(int _statistic)                    { m_statisticsMode = _statistic; }
    int getStatistic() const                             { return m_statisticsMode;       }

#endif
*/
private:

    /** Computes a LBPH model with images in src and
     *  corresponding labels in labels, possibly preserving
     *  old model data.
     */
    void train(cv::InputArrayOfArrays src, cv::InputArray labels, bool preserveData);

private:

    // NOTE: Do not use a d private internal container, this will crash OpenCV in cv::Algorithm::set()
    /*
    int                  m_grid_x;
    int                  m_grid_y;
    int                  m_radius;
    int                  m_neighbors;
    double               m_threshold;
    int                  m_statisticsMode;

    std::vector<cv::Mat> m_histograms;
    cv::Mat              m_labels;
    */

    int m_num_components;
    int m_threshold;

    std::vector<cv::Mat> m_src;
    std::vector<cv::Mat> m_projections;
    cv::Mat m_labels;
    cv::Mat m_eigenvectors;
    //cv::Mat m_eigenvalues;
    cv::Mat m_mean;
};

} // namespace FacesEngine
