void drawGround(int x, int y, float height) {
    glPushMatrix();
    glTranslatef(0, height, 0);
    glBegin(GL_QUADS);
    for (int i = 0; i < x; i += 1)
        for (int j = 0; j < y; j += 1) {
            glVertex3f(i, 0, j);
            glVertex3f(i + 1, 0, j);
            glVertex3f(i + 1, 0, j + 1);
            glVertex3f(i, 0, j + 1);
        }
    glEnd();
    glPopMatrix();
}