#include "requests.h"

err_t process_request(const request_t &request)
{
    err_t error_code = OK;

    static figure_t figure = init_figure();

    switch (request.action) {
        case LOAD:
            error_code = load_figure(figure, request.filename);
            break;
        case SAVE:
            error_code = save_figure(figure, request.filename);
            break;
        case DRAW:
            error_code = draw_figure(figure, request.canvas);
            break;
        case TRANSLATE:
            error_code = translate_figure(figure, request.change);
            break;
        case ROTATE:
            error_code = rotate_figure(figure, request.change);
            break;
        case SCALE:
            error_code = scale_figure(figure, request.change);
            break;
        case QUIT:
            free_figure(figure);
            break;
        default:
            error_code = ERR_UNKNOWN;
    }

    return error_code;
}
