
class Renderer {
public:
  virtual ~Renderer() = default;
  virtual void init() = 0;
  virtual void render() = 0;
};
