#include "editor_plugin_interface.h"

class CartoonPlugin: public QObject, public EditorPluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID EDIT_PLUGIN_INTERFACE_IID);
    Q_INTERFACES(EditorPluginInterface);

public:
    QString name() const;
    void edit(const cv::Mat &input, cv::Mat &output);

private:
    void smoothEdge(const cv::Mat &input, cv::Mat &output);
    void detectEdge(const cv::Mat &input, cv::Mat &output);
};
